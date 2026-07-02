#pragma once
#include <string>

class Cidade {
public:
    int id;
    std::string nome;

    Cidade() : id(0), nome("") {}
    Cidade(int id, const std::string& nome) : id(id), nome(nome) {}

    std::string serializar() const;
    static Cidade desserializar(const std::string& linha);
};
