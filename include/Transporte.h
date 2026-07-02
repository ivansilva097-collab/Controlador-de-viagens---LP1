#pragma once
#include <string>

enum class TipoTransporte { TERRESTRE, AQUATICO };

class Transporte {
public:
    int id;
    std::string nome;
    TipoTransporte tipo;
    int capacidade;
    double velocidade;       // km/h
    double distDescanco;     // km entre descansos
    double tempoDescanco;    // horas de descanso
    int cidadeAtualId;       // -1 = em viagem
    int viagemAtualId;       // -1 = sem viagem

    Transporte()
        : id(0), nome(""), tipo(TipoTransporte::TERRESTRE),
          capacidade(0), velocidade(0), distDescanco(0), tempoDescanco(0),
          cidadeAtualId(-1), viagemAtualId(-1) {}

    Transporte(int id, const std::string& nome, TipoTransporte tipo,
               int capacidade, double velocidade, double distDescanco, double tempoDescanco, int cidadeId)
        : id(id), nome(nome), tipo(tipo), capacidade(capacidade),
          velocidade(velocidade), distDescanco(distDescanco), tempoDescanco(tempoDescanco),
          cidadeAtualId(cidadeId), viagemAtualId(-1) {}

    std::string tipoStr() const { return tipo == TipoTransporte::TERRESTRE ? "TERRESTRE" : "AQUATICO"; }
    bool disponivel() const { return viagemAtualId == -1; }
    std::string serializar() const;
    static Transporte desserializar(const std::string& linha);
};
