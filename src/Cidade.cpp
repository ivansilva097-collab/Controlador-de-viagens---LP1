#include "Cidade.h"
#include <sstream>

std::string Cidade::serializar() const {
    return std::to_string(id) + "|" + nome;
}

Cidade Cidade::desserializar(const std::string& linha) {
    std::istringstream ss(linha);
    std::string token;
    Cidade c;
    std::getline(ss, token, '|'); c.id = std::stoi(token);
    std::getline(ss, c.nome);
    return c;
}
