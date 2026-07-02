#include "Viagem.h"
#include <sstream>

int Viagem::cidadeAtual() const {
    if (status == StatusViagem::CONCLUIDA) return destinoId;
    if (etapaAtual < (int)etapas.size())
        return etapas[etapaAtual].origemId;
    return destinoId;
}

std::string Viagem::statusStr() const {
    switch (status) {
        case StatusViagem::EM_ANDAMENTO: return "EM_ANDAMENTO";
        case StatusViagem::CONCLUIDA: return "CONCLUIDA";
        case StatusViagem::AGUARDANDO: return "AGUARDANDO";
    }
    return "DESCONHECIDO";
}

// Formato: id|transporteId|origemId|destinoId|etapaAtual|status|tempoTotal|distPercorrida|tempoDescancado|pass1,pass2,...|etapas(tId:ori:dest:dist:conc;...)
std::string Viagem::serializar() const {
    std::string s = std::to_string(id) + "|" +
                    std::to_string(transporteId) + "|" +
                    std::to_string(origemId) + "|" +
                    std::to_string(destinoId) + "|" +
                    std::to_string(etapaAtual) + "|" +
                    statusStr() + "|" +
                    std::to_string(tempoTotalHoras) + "|" +
                    std::to_string(distPercorrida) + "|" +
                    std::to_string(tempoDescancado) + "|";

    for (int i = 0; i < (int)passageirosIds.size(); i++) {
        if (i) s += ",";
        s += std::to_string(passageirosIds[i]);
    }
    s += "|";

    for (int i = 0; i < (int)etapas.size(); i++) {
        if (i) s += ";";
        const auto& e = etapas[i];
        s += std::to_string(e.trajetoId) + ":" +
             std::to_string(e.origemId) + ":" +
             std::to_string(e.destinoId) + ":" +
             std::to_string(e.distancia) + ":" +
             (e.concluida ? "1" : "0");
    }
    return s;
}

Viagem Viagem::desserializar(const std::string& linha) {
    std::istringstream ss(linha);
    std::string token;
    Viagem v;

    std::getline(ss, token, '|'); v.id = std::stoi(token);
    std::getline(ss, token, '|'); v.transporteId = std::stoi(token);
    std::getline(ss, token, '|'); v.origemId = std::stoi(token);
    std::getline(ss, token, '|'); v.destinoId = std::stoi(token);
    std::getline(ss, token, '|'); v.etapaAtual = std::stoi(token);
    std::getline(ss, token, '|');
    if (token == "EM_ANDAMENTO") v.status = StatusViagem::EM_ANDAMENTO;
    else if (token == "CONCLUIDA") v.status = StatusViagem::CONCLUIDA;
    else v.status = StatusViagem::AGUARDANDO;
    std::getline(ss, token, '|'); v.tempoTotalHoras = std::stod(token);
    std::getline(ss, token, '|'); v.distPercorrida  = std::stod(token);
    std::getline(ss, token, '|'); v.tempoDescancado = std::stod(token);

    // passageiros
    std::string passStr;
    std::getline(ss, passStr, '|');
    if (!passStr.empty()) {
        std::istringstream ps(passStr);
        std::string pid;
        while (std::getline(ps, pid, ','))
            v.passageirosIds.push_back(std::stoi(pid));
    }

    // etapas
    std::string etapasStr;
    std::getline(ss, etapasStr);
    if (!etapasStr.empty()) {
        std::istringstream es(etapasStr);
        std::string etoken;
        while (std::getline(es, etoken, ';')) {
            std::istringstream ef(etoken);
            std::string f;
            Etapa e;
            std::getline(ef, f, ':'); e.trajetoId = std::stoi(f);
            std::getline(ef, f, ':'); e.origemId  = std::stoi(f);
            std::getline(ef, f, ':'); e.destinoId = std::stoi(f);
            std::getline(ef, f, ':'); e.distancia = std::stod(f);
            std::getline(ef, f); e.concluida  = (f == "1");
            v.etapas.push_back(e);
        }
    }
    return v;
}
