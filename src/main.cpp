#include "ControladorDeTransito.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <limits>

static ControladorDeTransito ctrl;
static const std::string DATA_DIR = "data";

static int lerInt(const std::string& prompt) {
    int v;
    std::cout << prompt;
    while (!(std::cin >> v)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  Entrada inválida. " << prompt;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return v;
}
static double lerDouble(const std::string& prompt) {
    double v;
    std::cout << prompt;
    while (!(std::cin >> v)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  Entrada inválida. " << prompt;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return v;
}
static std::string lerString(const std::string& prompt) {
    std::string s;
    std::cout << prompt;
    std::getline(std::cin, s);
    return s;
}
static int lerOpcao() {
    int v;
    while (!(std::cin >> v)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  Opcao invalida, digite um numero.\nOpcao: ";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return v;
}
static TipoTrajeto lerTipoTrajeto() {
    int op = lerInt("  Tipo (1=Terrestre / 2=Aquatico): ");
    return op == 2 ? TipoTrajeto::AQUATICO : TipoTrajeto::TERRESTRE;
}
static TipoTransporte lerTipoTransporte() {
    int op = lerInt("  Tipo (1=Terrestre / 2=Aquatico): ");
    return op == 2 ? TipoTransporte::AQUATICO : TipoTransporte::TERRESTRE;
}
static std::vector<int> lerListaInts(const std::string& prompt) {
    std::cout << prompt;
    std::string linha; std::getline(std::cin, linha);
    std::vector<int> lista;
    std::istringstream ss(linha);
    int v;
    while (ss >> v) lista.push_back(v);
    return lista;
}

void menuCadastros() {
    while (true) {
        std::cout << "\n--- CADASTROS ---\n"
                  << " 1. Cadastrar cidade\n"
                  << " 2. Cadastrar trajeto\n"
                  << " 3. Cadastrar transporte\n"
                  << " 4. Cadastrar passageiro\n"
                  << " 0. Voltar\n"
                  << "Opcao: ";
        int op = lerOpcao();
        if (op == 0) break;
        if (op == 1) {
            auto nome = lerString("  Nome da cidade: ");
            ctrl.cadastrarCidade(nome);
        } else if (op == 2) {
            ctrl.listarCidades();
            int ori = lerInt("  ID Origem: ");
            int dst = lerInt("  ID Destino: ");
            auto tipo = lerTipoTrajeto();
            double dist = lerDouble("  Distancia (km): ");
            ctrl.cadastrarTrajeto(ori, dst, tipo, dist);
        } else if (op == 3) {
            ctrl.listarCidades();
            auto nome = lerString("  Nome do transporte: ");
            auto tipo = lerTipoTransporte();
            int cap    = lerInt("  Capacidade (passageiros): ");
            double vel = lerDouble("  Velocidade (km/h): ");
            double dd  = lerDouble("  Distancia entre descansos (km, 0=sem descanso): ");
            double td  = lerDouble("  Tempo de descanso (horas): ");
            int cid    = lerInt("  ID da cidade base: ");
            ctrl.cadastrarTransporte(nome, tipo, cap, vel, dd, td, cid);
        } else if (op == 4) {
            ctrl.listarCidades();
            auto nome = lerString("  Nome do passageiro: ");
            int cid   = lerInt("  ID da cidade inicial: ");
            ctrl.cadastrarPassageiro(nome, cid);
        }
    }
}

void menuViagens() {
    while (true) {
        std::cout << "\n--- VIAGENS ---\n"
                  << " 1. Iniciar viagem\n"
                  << " 2. Avancar tempo\n"
                  << " 3. Viagens em andamento\n"
                  << " 4. Melhor caminho (simulacao)\n"
                  << " 0. Voltar\n"
                  << "Opcao: ";
        int op = lerOpcao();
        if (op == 0) break;
        if (op == 1) {
            ctrl.listarTransportes();
            int tid = lerInt("  ID do transporte: ");
            ctrl.listarPassageiros();
            auto pids = lerListaInts("  IDs dos passageiros (separados por espaco): ");
            ctrl.listarCidades();
            int dst  = lerInt("  ID da cidade destino: ");
            ctrl.iniciarViagem(tid, pids, dst);
        } else if (op == 2) {
            double h = lerDouble("  Quantas horas avancar: ");
            ctrl.avancarTempo(h);
        } else if (op == 3) {
            ctrl.viagensEmAndamento();
        } else if (op == 4) {
            ctrl.listarCidades();
            int ori = lerInt("  ID Origem: ");
            int dst = lerInt("  ID Destino: ");
            auto tipo = lerTipoTransporte();
            ctrl.melhorCaminho(ori, dst, tipo);
        }
    }
}

void menuConsultas() {
    while (true) {
        std::cout << "\n--- CONSULTAS E RELATORIOS ---\n"
                  << " 1. Listar cidades\n"
                  << " 2. Listar trajetos\n"
                  << " 3. Listar transportes\n"
                  << " 4. Listar passageiros\n"
                  << " 5. Listar todas as viagens\n"
                  << " 6. Localizar passageiro\n"
                  << " 7. Localizar transporte\n"
                  << " 8. Cidades mais visitadas\n"
                  << " 0. Voltar\n"
                  << "Opcao: ";
        int op = lerOpcao();
        if (op == 0) break;
        if (op == 1) ctrl.listarCidades();
        else if (op == 2) ctrl.listarTrajetos();
        else if (op == 3) ctrl.listarTransportes();
        else if (op == 4) ctrl.listarPassageiros();
        else if (op == 5) ctrl.listarViagens();
        else if (op == 6) { int id = lerInt("  ID do passageiro: "); ctrl.localizarPassageiro(id); }
        else if (op == 7) { int id = lerInt("  ID do transporte: "); ctrl.localizarTransporte(id); }
        else if (op == 8) ctrl.cidadesMaisVisitadas();
    }
}

int main() {
    std::cout << "CVS - Controlador de Viagens";

    // tenta carregar dados salvos
    ctrl.carregar(DATA_DIR);

    while (true) {
        std::cout << "\n====== MENU PRINCIPAL ======\n"
                  << " 1. Cadastros\n"
                  << " 2. Viagens\n"
                  << " 3. Consultas e Relatorios\n"
                  << " 4. Salvar dados\n"
                  << " 0. Sair\n"
                  << "Opcao: ";
        int op = lerOpcao();
        if (op == 0) {
            ctrl.salvar(DATA_DIR);
            std::cout << "Ate logo!\n";
            break;
        }
        if (op == 1) menuCadastros();
        else if (op == 2) menuViagens();
        else if (op == 3) menuConsultas();
        else if (op == 4) ctrl.salvar(DATA_DIR);
    }
    return 0;
}
