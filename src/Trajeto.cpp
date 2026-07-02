#include "Trajeto.h"
#include <sstream>

std::string Trajeto::serializar() const {
    return std::to_string(id) + "|" +
           std::to_string(origemId) + "|" +
           std::to_string(destinoId) + "|" +
           tipoStr() + "|" +
           std::to_string(distancia);
}

Trajeto Trajeto::desserializar(const std::string& linha) {
    std::istringstream ss(linha);
    std::string token;
    Trajeto t;
    std::getline(ss, token, '|'); t.id = std::stoi(token);
    std::getline(ss, token, '|'); t.origemId = std::stoi(token);
    std::getline(ss, token, '|'); t.destinoId = std::stoi(token);
    std::getline(ss, token, '|'); t.tipo = (token == "TERRESTRE") ? TipoTrajeto::TERRESTRE : TipoTrajeto::AQUATICO;
    std::getline(ss, token);      t.distancia = std::stod(token);
    return t;
}
