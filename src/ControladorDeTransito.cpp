#include "ControladorDeTransito.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <queue>
#include <map>
#include <iomanip>

Cidade* ControladorDeTransito::buscarCidade(int id) { for (auto& c : cidades) if (c.id==id) return &c; return nullptr; }
Trajeto* ControladorDeTransito::buscarTrajeto(int id) { for (auto& t : trajetos) if (t.id==id) return &t; return nullptr; }
Transporte* ControladorDeTransito::buscarTransporte(int id) { for (auto& t : transportes) if (t.id==id) return &t; return nullptr; }
Passageiro* ControladorDeTransito::buscarPassageiro(int id) { for (auto& p : passageiros) if (p.id==id) return &p; return nullptr; }
Viagem* ControladorDeTransito::buscarViagem(int id) { for (auto& v : viagens) if (v.id==id) return &v; return nullptr; }

const Cidade* ControladorDeTransito::buscarCidade(int id) const { for (const auto& c : cidades) if (c.id==id) return &c; return nullptr; }
const Trajeto* ControladorDeTransito::buscarTrajeto(int id) const { for (const auto& t : trajetos) if (t.id==id) return &t; return nullptr; }
const Transporte* ControladorDeTransito::buscarTransporte(int id) const { for (const auto& t : transportes) if (t.id==id) return &t; return nullptr; }
const Passageiro* ControladorDeTransito::buscarPassageiro(int id) const { for (const auto& p : passageiros) if (p.id==id) return &p; return nullptr; }
const Viagem* ControladorDeTransito::buscarViagem(int id) const { for (const auto& v : viagens) if (v.id==id) return &v; return nullptr; }

Cidade* ControladorDeTransito::buscarCidadePorNome(const std::string& nome) {
    for (auto& c : cidades) if (c.nome == nome) return &c;
    return nullptr;
}
Transporte* ControladorDeTransito::buscarTransportePorNome(const std::string& nome) {
    for (auto& t : transportes) if (t.nome == nome) return &t;
    return nullptr;
}

void ControladorDeTransito::cadastrarCidade(const std::string& nome) {
    if (buscarCidadePorNome(nome)) {
        std::cout << "[AVISO] Cidade '" << nome << "' ja existe.\n";
        return;
    }
    cidades.emplace_back(proximoIdCidade++, nome);
    std::cout << "[OK] Cidade cadastrada: " << nome << " (ID=" << cidades.back().id << ")\n";
}

void ControladorDeTransito::cadastrarTrajeto(int origemId, int destinoId, TipoTrajeto tipo, double distancia) {
    if (!buscarCidade(origemId) || !buscarCidade(destinoId)) {
        std::cout << "[ERRO] Cidade de origem ou destino nao encontrada.\n";
        return;
    }
    if (distancia <= 0) { std::cout << "[ERRO] Distancia deve ser positiva.\n"; return; }
    trajetos.emplace_back(proximoIdTrajeto++, origemId, destinoId, tipo, distancia);
    std::cout << "[OK] Trajeto cadastrado ID=" << trajetos.back().id << "\n";
}

void ControladorDeTransito::cadastrarTransporte(const std::string& nome, TipoTransporte tipo,
    int capacidade, double velocidade, double distDescanco, double tempoDescanco, int cidadeId) {
    if (!buscarCidade(cidadeId)) { std::cout << "[ERRO] Cidade nao encontrada.\n"; return; }
    transportes.emplace_back(proximoIdTransporte++, nome, tipo, capacidade, velocidade,
                             distDescanco, tempoDescanco, cidadeId);
    std::cout << "[OK] Transporte cadastrado: " << nome << " (ID=" << transportes.back().id << ")\n";
}

void ControladorDeTransito::cadastrarPassageiro(const std::string& nome, int cidadeId) {
    if (!buscarCidade(cidadeId)) { std::cout << "[ERRO] Cidade nao encontrada.\n"; return; }
    passageiros.emplace_back(proximoIdPassageiro++, nome, cidadeId);
    std::cout << "[OK] Passageiro cadastrado: " << nome << " (ID=" << passageiros.back().id << ")\n";
}

// Retorna os trajetoIds do melhor caminho (vazio se não há rota)
// por tipoTransporte impede que um transporte terrestre use rota aquática
std::vector<int> ControladorDeTransito::dijkstra(int origemId, int destinoId,
                                                  TipoTransporte tipoTransporte) const {
    TipoTrajeto tipoTrajeto = (tipoTransporte == TipoTransporte::TERRESTRE)
                              ? TipoTrajeto::TERRESTRE : TipoTrajeto::AQUATICO;

    // dist = menor distância até cada cidade; anterior/trajetoUsado = para reconstruir a rota
    std::map<int, double> dist;
    std::map<int, int> anterior;
    std::map<int, int> trajetoUsado;
    for (const auto& c : cidades) dist[c.id] = std::numeric_limits<double>::infinity();
    dist[origemId] = 0;

    using Par = std::pair<double, int>; // (distancia acumulada, cidadeId)
    std::priority_queue<Par, std::vector<Par>, std::greater<Par>> pq;
    pq.push({0.0, origemId});

    while (!pq.empty()) {
        Par top = pq.top(); pq.pop();
        double d = top.first;
        int u = top.second;

        if (d > dist[u]) continue;
        if (u == destinoId) break;

        for (const auto& t : trajetos) {
            if (t.tipo != tipoTrajeto) continue;
            int v = -1;
            if (t.origemId == u)  v = t.destinoId;
            else continue;
            double nd = dist[u] + t.distancia;
            if (nd < dist[v]) {
                dist[v] = nd;
                anterior[v] = u;
                trajetoUsado[v] = t.id;
                pq.push({nd, v});
            }
        }
    }

    if (dist[destinoId] == std::numeric_limits<double>::infinity())
        return {}; // destino inalcançável com o tipo de transporte solicitado

    std::vector<int> caminho;
    int cur = destinoId;
    while (cur != origemId) {
        caminho.push_back(trajetoUsado[cur]);
        cur = anterior[cur];
    }
    std::reverse(caminho.begin(), caminho.end());
    return caminho;
}

int ControladorDeTransito::iniciarViagem(int transporteId, const std::vector<int>& passageirosIds, int destinoId) {
    Transporte* tr = buscarTransporte(transporteId);
    if (!tr) { std::cout << "[ERRO] Transporte nao encontrado.\n"; return -1; }
    if (!tr->disponivel()) { std::cout << "[ERRO] Transporte ja em viagem.\n"; return -1; }
    if (!buscarCidade(destinoId)) { std::cout << "[ERRO] Cidade destino nao encontrada.\n"; return -1; }
    if (tr->cidadeAtualId == -1) { std::cout << "[ERRO] Transporte sem cidade base.\n"; return -1; }

    if ((int)passageirosIds.size() > tr->capacidade) {
        std::cout << "[ERRO] Passageiros excedem capacidade do transporte.\n"; return -1;
    }
    for (int pid : passageirosIds) {
        Passageiro* p = buscarPassageiro(pid);
        if (!p) { std::cout << "[ERRO] Passageiro " << pid << " nao encontrado.\n"; return -1; }
        if (!p->disponivel()) { std::cout << "[ERRO] Passageiro " << p->nome << " ja em viagem.\n"; return -1; }
        if (p->cidadeAtualId != tr->cidadeAtualId) {
            std::cout << "[ERRO] Passageiro " << p->nome << " nao esta na cidade do transporte.\n"; return -1;
        }
    }

    // Dijkstra monta a sequência de etapas (direta ou com conexão)
    auto trajetoIds = dijkstra(tr->cidadeAtualId, destinoId, tr->tipo);
    if (trajetoIds.empty()) {
        std::cout << "[ERRO] Nao existe caminho entre as cidades para esse tipo de transporte.\n";
        return -1;
    }

    // Monta a viagem com uma etapa para cada trajeto da rota encontrada
    Viagem v;
    v.id = proximoIdViagem++;
    v.transporteId = transporteId;
    v.passageirosIds = passageirosIds;
    v.origemId = tr->cidadeAtualId;
    v.destinoId = destinoId;
    v.status = StatusViagem::EM_ANDAMENTO;

    for (int tid : trajetoIds) {
        Trajeto* tj = buscarTrajeto(tid);
        v.etapas.emplace_back(tid, tj->origemId, tj->destinoId, tj->distancia);
    }
    v.etapaAtual = 0;
    v.tempoTotalHoras = 0;
    v.distPercorrida = 0;
    v.tempoDescancado = 0;

    viagens.push_back(v);

    // Saem do "mapa" (cidadeAtualId=-1) até a viagem concluir
    tr->cidadeAtualId = -1;
    tr->viagemAtualId = v.id;
    for (int pid : passageirosIds) {
        Passageiro* p = buscarPassageiro(pid);
        p->cidadeAtualId = -1;
        p->viagemAtualId = v.id;
    }

    std::cout << "[OK] Viagem " << v.id << " iniciada! Rota com " << v.etapas.size() << " etapa(s).\n";
    return v.id;
}

// Avança N horas em todas as viagens em andamento, em partes
void ControladorDeTransito::avancarTempo(double horas) {
    std::cout << "\n[TEMPO] Avancando " << horas << " hora(s)...\n";

    for (auto& v : viagens) {
        if (v.status != StatusViagem::EM_ANDAMENTO) continue;
        Transporte* tr = buscarTransporte(v.transporteId);
        if (!tr) continue;

        double horasRestantes = horas;

        while (horasRestantes > 0 && v.etapaAtual < (int)v.etapas.size()) {
            Etapa& etapa = v.etapas[v.etapaAtual];
            double distEtapaRestante = etapa.distancia - v.distPercorrida;

            // Km até o próximo descanso
            double distAteDescanco = tr->distDescanco - std::fmod(v.distPercorrida, tr->distDescanco);
            if (tr->distDescanco <= 0) distAteDescanco = distEtapaRestante;

            // Percorre o menor entre: tempo disponível, fim da etapa, ou próximo descanso
            double distPercorrer = std::min({horasRestantes * tr->velocidade,
                                             distEtapaRestante,
                                             distAteDescanco});
            double horasGastas = distPercorrer / tr->velocidade;
            v.distPercorrida += distPercorrer;
            v.tempoTotalHoras += horasGastas;
            horasRestantes -= horasGastas;

            // Parou num múltiplo de distDescanco? Descansa automaticamente
            if (tr->distDescanco > 0 &&
                std::fmod(v.distPercorrida, tr->distDescanco) < 0.001 &&
                v.distPercorrida < etapa.distancia &&
                horasRestantes > 0) {
                double descanco = std::min(horasRestantes, tr->tempoDescanco);
                v.tempoTotalHoras += descanco;
                horasRestantes -= descanco;
            }

            // Etapa concluída: avança pra próxima ou finaliza a viagem
            if (v.distPercorrida >= etapa.distancia - 0.001) {
                etapa.concluida = true;
                v.distPercorrida = 0;
                finalizarEtapa(v);
                if (v.status == StatusViagem::CONCLUIDA) break;
            }
        }
    }
}

void ControladorDeTransito::finalizarEtapa(Viagem& v) {
    Transporte* tr = buscarTransporte(v.transporteId);
    Etapa& etapa = v.etapas[v.etapaAtual];

    v.etapaAtual++;

    if (v.etapaAtual >= (int)v.etapas.size()) {
        // Viagem completa: transporte e passageiros reaparecem no destino final
        v.status = StatusViagem::CONCLUIDA;
        if (tr) { tr->viagemAtualId = -1; tr->cidadeAtualId = v.destinoId; }
        for (int pid : v.passageirosIds) {
            Passageiro* p = buscarPassageiro(pid);
            if (p) { p->viagemAtualId = -1; p->cidadeAtualId = v.destinoId; }
        }
        const Cidade* dest = buscarCidade(v.destinoId);
        std::cout << "  [CHEGADA] Viagem " << v.id << " concluida em "
                  << (dest ? dest->nome : "?") << " ("
                  << std::fixed << std::setprecision(1) << v.tempoTotalHoras << "h no total)\n";
    } else {
        const Cidade* c = buscarCidade(etapa.destinoId);
        std::cout << "  [CONEXAO] Viagem " << v.id << " chegou em "
                  << (c ? c->nome : "?") << ", proxima etapa...\n";
    }
}

void ControladorDeTransito::listarCidades() const {
    std::cout << "\n=== CIDADES (" << cidades.size() << ") ===\n";
    for (const auto& c : cidades)
        std::cout << "  [" << c.id << "] " << c.nome << "\n";
}

void ControladorDeTransito::listarTrajetos() const {
    std::cout << "\n=== TRAJETOS (" << trajetos.size() << ") ===\n";
    for (const auto& t : trajetos) {
        const Cidade* o = buscarCidade(t.origemId);
        const Cidade* d = buscarCidade(t.destinoId);
        std::cout << "  [" << t.id << "] "
                  << (o ? o->nome : "?") << " -> " << (d ? d->nome : "?")
                  << " | " << t.tipoStr()
                  << " | " << t.distancia << " km\n";
    }
}

void ControladorDeTransito::listarTransportes() const {
    std::cout << "\n=== TRANSPORTES (" << transportes.size() << ") ===\n";
    for (const auto& t : transportes) {
        const Cidade* c = buscarCidade(t.cidadeAtualId);
        std::cout << "  [" << t.id << "] " << t.nome
                  << " | " << t.tipoStr()
                  << " | Cap:" << t.capacidade
                  << " | " << t.velocidade << "km/h"
                  << " | Desc a cada " << t.distDescanco << "km (" << t.tempoDescanco << "h)"
                  << " | Local: " << (c ? c->nome : (t.viagemAtualId != -1 ? "Em viagem #"+std::to_string(t.viagemAtualId) : "?")) << "\n";
    }
}

void ControladorDeTransito::listarPassageiros() const {
    std::cout << "\n=== PASSAGEIROS (" << passageiros.size() << ") ===\n";
    for (const auto& p : passageiros) {
        const Cidade* c = buscarCidade(p.cidadeAtualId);
        std::cout << "  [" << p.id << "] " << p.nome
                  << " | " << (c ? c->nome : (p.viagemAtualId != -1 ? "Em viagem #"+std::to_string(p.viagemAtualId) : "?")) << "\n";
    }
}

std::string ControladorDeTransito::formatarLinhaViagem(const Viagem& v, bool comTransporte) const {
    const Cidade* o = buscarCidade(v.origemId);
    const Cidade* d = buscarCidade(v.destinoId);
    int etapaExibida = std::min(v.etapaAtual + 1, (int)v.etapas.size());

    std::ostringstream s;
    s << (o ? o->nome : "?") << " -> " << (d ? d->nome : "?");
    if (comTransporte) {
        const Transporte* tr = buscarTransporte(v.transporteId);
        s << " | Transporte: " << (tr ? tr->nome : "?");
    } else {
        s << " | Status: " << v.statusStr();
    }
    s << " | Etapa: " << etapaExibida << "/" << v.etapas.size()
      << " | Passageiros: " << v.passageirosIds.size()
      << " | Tempo: " << std::fixed << std::setprecision(1) << v.tempoTotalHoras << "h";
    return s.str();
}

void ControladorDeTransito::listarViagens() const {
    std::cout << "\n=== VIAGENS (" << viagens.size() << ") ===\n";
    for (const auto& v : viagens)
        std::cout << "  [" << v.id << "] " << formatarLinhaViagem(v, false) << "\n";
}

void ControladorDeTransito::localizarPassageiro(int id) const {
    const Passageiro* p = buscarPassageiro(id);
    if (!p) { std::cout << "[ERRO] Passageiro nao encontrado.\n"; return; }
    std::cout << "\nPassageiro: " << p->nome << "\n";
    if (p->cidadeAtualId != -1) {
        const Cidade* c = buscarCidade(p->cidadeAtualId);
        std::cout << "  Localização: " << (c ? c->nome : "?") << "\n";
    } else {
        const Viagem* v = buscarViagem(p->viagemAtualId);
        if (v) {
            const Transporte* tr = buscarTransporte(v->transporteId);
            const Cidade* origemAtual = buscarCidade(v->cidadeAtual());
            const Cidade* destinoFinal = buscarCidade(v->destinoId);
            std::cout << "  Em viagem #" << v->id
                      << " | Origem desta etapa: " << (origemAtual ? origemAtual->nome : "?")
                      << " -> Destino final: " << (destinoFinal ? destinoFinal->nome : "?")
                      << " | Transporte: " << (tr ? tr->nome : "?") << "\n";
            if (v->etapaAtual < (int)v->etapas.size()) {
                const Cidade* eD = buscarCidade(v->etapas[v->etapaAtual].destinoId);
                std::cout << "  Proximo ponto: " << (eD ? eD->nome : "?")
                          << " | Dist percorrida na etapa atual: "
                          << std::fixed << std::setprecision(1) << v->distPercorrida << " km\n";
            }
        } else {
            std::cout << "  Em viagem #" << p->viagemAtualId << " (dados da viagem nao encontrados)\n";
        }
    }
}

void ControladorDeTransito::localizarTransporte(int id) const {
    const Transporte* t = buscarTransporte(id);
    if (!t) { std::cout << "[ERRO] Transporte nao encontrado.\n"; return; }
    std::cout << "\nTransporte: " << t->nome << "\n";
    if (t->cidadeAtualId != -1) {
        const Cidade* c = buscarCidade(t->cidadeAtualId);
        std::cout << "  Localização: " << (c ? c->nome : "?") << "\n";
    } else {
        const Viagem* v = buscarViagem(t->viagemAtualId);
        if (v) {
            const Cidade* origemAtual = buscarCidade(v->cidadeAtual());
            const Cidade* destinoFinal = buscarCidade(v->destinoId);
            std::cout << "  Em viagem #" << v->id
                      << " | Origem desta etapa: " << (origemAtual ? origemAtual->nome : "?")
                      << " -> Destino final: " << (destinoFinal ? destinoFinal->nome : "?")
                      << " | Transporte: " << t->nome
                      << " | Passageiros a bordo: " << v->passageirosIds.size() << "\n";
        } else {
            std::cout << "  Em viagem #" << t->viagemAtualId << " (dados da viagem nao encontrados)\n";
        }
    }
}

void ControladorDeTransito::viagensEmAndamento() const {
    std::cout << "\n=== VIAGENS EM ANDAMENTO ===\n";
    bool alguma = false;
    for (const auto& v : viagens) {
        if (v.status != StatusViagem::EM_ANDAMENTO) continue;
        alguma = true;
        std::cout << "  Viagem #" << v.id << " | " << formatarLinhaViagem(v, true) << " decorridas\n";
    }
    if (!alguma) std::cout << "  Nenhuma viagem em andamento.\n";
}

void ControladorDeTransito::cidadesMaisVisitadas() const {
    std::map<int, int> visitas;
    for (const auto& v : viagens) {
        if (v.status != StatusViagem::CONCLUIDA) continue;
        for (const auto& e : v.etapas) {
            if (e.concluida) visitas[e.destinoId]++;
        }
    }
    std::cout << "\n=== CIDADES MAIS VISITADAS ===\n";
    std::vector<std::pair<int,int>> ranking(visitas.begin(), visitas.end());
    std::sort(ranking.begin(), ranking.end(), [](auto& a, auto& b){ return a.second > b.second; });
    if (ranking.empty()) { std::cout << "  Sem viagens concluidas.\n"; return; }
    for (auto& par : ranking) {
        int cid = par.first;
        int n = par.second;
        const Cidade* c = buscarCidade(cid);
        std::cout << "  " << (c?c->nome:"?") << ": " << n << " visita(s)\n";
    }
}

void ControladorDeTransito::melhorCaminho(int origemId, int destinoId, TipoTransporte tipo) const {
    auto trajetoIds = dijkstra(origemId, destinoId, tipo);
    const Cidade* o = buscarCidade(origemId);
    const Cidade* d = buscarCidade(destinoId);
    std::cout << "\n=== MELHOR CAMINHO (Dijkstra) ===\n";
    std::cout << "De: " << (o?o->nome:"?") << " -> " << (d?d->nome:"?")
              << " | Tipo: " << (tipo==TipoTransporte::TERRESTRE?"TERRESTRE":"AQUATICO") << "\n";
    if (trajetoIds.empty()) {
        std::cout << "  Nenhum caminho encontrado!\n";
        return;
    }
    double total = 0;
    for (int tid : trajetoIds) {
        const Trajeto* t = buscarTrajeto(tid);
        if (!t) continue;
        const Cidade* co = buscarCidade(t->origemId);
        const Cidade* cd = buscarCidade(t->destinoId);
        std::cout << "  " << (co?co->nome:"?") << " -> " << (cd?cd->nome:"?")
                  << " (" << t->distancia << " km)\n";
        total += t->distancia;
    }
    std::cout << "  TOTAL: " << total << " km\n";
}

void ControladorDeTransito::salvar(const std::string& dir) const {
    auto salvarArquivo = [&](const std::string& nome, auto& lista) {
        std::ofstream f(dir + "/" + nome);
        for (const auto& item : lista) f << item.serializar() << "\n";
    };
    salvarArquivo("cidades.dat",     cidades);
    salvarArquivo("trajetos.dat",    trajetos);
    salvarArquivo("transportes.dat", transportes);
    salvarArquivo("passageiros.dat", passageiros);
    salvarArquivo("viagens.dat",     viagens);
    std::cout << "[OK] Dados salvos em '" << dir << "'.\n";
}

void ControladorDeTransito::carregar(const std::string& dir) {
    auto carregarArquivo = [&](const std::string& nome, auto& lista, auto& proximoId) {
        using T = typename std::decay<decltype(lista)>::type::value_type;
        std::ifstream f(dir + "/" + nome);
        if (!f) return;
        std::string linha;
        while (std::getline(f, linha)) {
            if (!linha.empty()) {
                auto item = T::desserializar(linha);
                lista.push_back(item);
                if (item.id >= proximoId) proximoId = item.id + 1;
            }
        }
    };
    cidades.clear(); trajetos.clear(); transportes.clear(); passageiros.clear(); viagens.clear();
    carregarArquivo("cidades.dat",     cidades,     proximoIdCidade);
    carregarArquivo("trajetos.dat",    trajetos,    proximoIdTrajeto);
    carregarArquivo("transportes.dat", transportes, proximoIdTransporte);
    carregarArquivo("passageiros.dat", passageiros, proximoIdPassageiro);
    carregarArquivo("viagens.dat",     viagens,     proximoIdViagem);
    std::cout << "[OK] Dados carregados de '" << dir << "'.\n";
}
