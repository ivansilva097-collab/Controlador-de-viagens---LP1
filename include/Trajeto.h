#pragma once
#include <string>

enum class TipoTrajeto { TERRESTRE, AQUATICO };

class Trajeto {
public:
    int id;
    int origemId;
    int destinoId;
    TipoTrajeto tipo;
    double distancia; // km

    Trajeto() : id(0), origemId(0), destinoId(0), tipo(TipoTrajeto::TERRESTRE), distancia(0) {}
    Trajeto(int id, int origemId, int destinoId, TipoTrajeto tipo, double distancia)
        : id(id), origemId(origemId), destinoId(destinoId), tipo(tipo), distancia(distancia) {}

    std::string tipoStr() const { return tipo == TipoTrajeto::TERRESTRE ? "TERRESTRE" : "AQUATICO"; }
    std::string serializar() const;
    static Trajeto desserializar(const std::string& linha);
};
