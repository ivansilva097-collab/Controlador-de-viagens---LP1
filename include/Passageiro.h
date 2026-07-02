#pragma once
#include <string>

class Passageiro {
public:
    int id;
    std::string nome;
    int cidadeAtualId; // -1 = em viagem
    int viagemAtualId; // -1 = sem viagem

    Passageiro() : id(0), nome(""), cidadeAtualId(-1), viagemAtualId(-1) {}
    Passageiro(int id, const std::string& nome, int cidadeId)
        : id(id), nome(nome), cidadeAtualId(cidadeId), viagemAtualId(-1) {}

    bool disponivel() const { return viagemAtualId == -1; }
    std::string serializar() const;
    static Passageiro desserializar(const std::string& linha);
};
