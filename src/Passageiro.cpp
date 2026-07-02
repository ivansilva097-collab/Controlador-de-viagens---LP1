#include "Passageiro.h"
#include <sstream>

std::string Passageiro::serializar() const {
    return std::to_string(id) + "|" +
           nome + "|" +
           std::to_string(cidadeAtualId) + "|" +
           std::to_string(viagemAtualId);
}

Passageiro Passageiro::desserializar(const std::string& linha) {
    std::istringstream ss(linha);
    std::string token;
    Passageiro p;
    std::getline(ss, token, '|'); p.id = std::stoi(token);
    std::getline(ss, p.nome,     '|');
    std::getline(ss, token, '|'); p.cidadeAtualId = std::stoi(token);
    std::getline(ss, token);      p.viagemAtualId = std::stoi(token);
    return p;
}
