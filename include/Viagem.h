#pragma once
#include <string>
#include <vector>

enum class StatusViagem { EM_ANDAMENTO, CONCLUIDA, AGUARDANDO };

struct Etapa {
    int trajetoId;
    int origemId;
    int destinoId;
    double distancia;
    bool concluida;

    Etapa() : trajetoId(0), origemId(0), destinoId(0), distancia(0), concluida(false) {}
    Etapa(int tId, int ori, int dest, double dist)
        : trajetoId(tId), origemId(ori), destinoId(dest), distancia(dist), concluida(false) {}
};

class Viagem {
public:
    int id;
    int transporteId;
    std::vector<int> passageirosIds;
    int origemId;
    int destinoId;
    std::vector<Etapa> etapas;
    int etapaAtual;
    StatusViagem status;
    double tempoTotalHoras; // tempo acumulado
    double distPercorrida; // distância percorrida na etapa atual
    double tempoDescancado; // tempo já descansado na etapa atual

    Viagem()
        : id(0), transporteId(0), origemId(0), destinoId(0),
          etapaAtual(0), status(StatusViagem::EM_ANDAMENTO),
          tempoTotalHoras(0), distPercorrida(0), tempoDescancado(0) {}

    bool concluida() const { return status == StatusViagem::CONCLUIDA; }
    int cidadeAtual() const;
    std::string statusStr() const;
    std::string serializar() const;
    static Viagem desserializar(const std::string& linha);
};
