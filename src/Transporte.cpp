#include "Transporte.h"
#include <sstream>

std::string Transporte::serializar() const {
    return std::to_string(id) + "|" +
           nome + "|" +
           tipoStr() + "|" +
           std::to_string(capacidade) + "|" +
           std::to_string(velocidade) + "|" +
           std::to_string(distDescanco) + "|" +
           std::to_string(tempoDescanco) + "|" +
           std::to_string(cidadeAtualId) + "|" +
           std::to_string(viagemAtualId);
}

Transporte Transporte::desserializar(const std::string& linha) {
    std::istringstream ss(linha);
    std::string token;
    Transporte t;
    std::getline(ss, token, '|'); t.id = std::stoi(token);
    std::getline(ss, t.nome,     '|');
    std::getline(ss, token, '|'); t.tipo = (token == "TERRESTRE") ? TipoTransporte::TERRESTRE : TipoTransporte::AQUATICO;
    std::getline(ss, token, '|'); t.capacidade = std::stoi(token);
    std::getline(ss, token, '|'); t.velocidade = std::stod(token);
    std::getline(ss, token, '|'); t.distDescanco = std::stod(token);
    std::getline(ss, token, '|'); t.tempoDescanco = std::stod(token);
    std::getline(ss, token, '|'); t.cidadeAtualId = std::stoi(token);
    std::getline(ss, token);      t.viagemAtualId = std::stoi(token);
    return t;
}
