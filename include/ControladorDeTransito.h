#pragma once
#include "Cidade.h"
#include "Trajeto.h"
#include "Transporte.h"
#include "Passageiro.h"
#include "Viagem.h"
#include <vector>
#include <map>
#include <string>
#include <limits>

class ControladorDeTransito {
private:
    std::vector<Cidade> cidades;
    std::vector<Trajeto> trajetos;
    std::vector<Transporte> transportes;
    std::vector<Passageiro> passageiros;
    std::vector<Viagem> viagens;

    int proximoIdCidade = 1;
    int proximoIdTrajeto = 1;
    int proximoIdTransporte = 1;
    int proximoIdPassageiro = 1;
    int proximoIdViagem = 1;

    // Dijkstra — retorna sequência de trajetoIds (melhor caminho por distância)
    std::vector<int> dijkstra(int origemId, int destinoId, TipoTransporte tipoTransporte) const;

    Cidade* buscarCidade(int id);
    Trajeto* buscarTrajeto(int id);
    Transporte* buscarTransporte(int id);
    Passageiro* buscarPassageiro(int id);
    Viagem* buscarViagem(int id);

    const Cidade* buscarCidade(int id) const;
    const Trajeto* buscarTrajeto(int id) const;
    const Transporte* buscarTransporte(int id) const;
    const Passageiro* buscarPassageiro(int id) const;
    const Viagem* buscarViagem(int id) const;

    Cidade* buscarCidadePorNome(const std::string& nome);
    Transporte* buscarTransportePorNome(const std::string& nome);

    std::string formatarLinhaViagem(const Viagem& v, bool comTransporte) const;

public:
    // Cadastros
    void cadastrarCidade(const std::string& nome);
    void cadastrarTrajeto(int origemId, int destinoId, TipoTrajeto tipo, double distancia);
    void cadastrarTransporte(const std::string& nome, TipoTransporte tipo, int capacidade,
                             double velocidade, double distDescanco, double tempoDescanco, int cidadeId);
    void cadastrarPassageiro(const std::string& nome, int cidadeId);

    // Viagens
    int  iniciarViagem(int transporteId, const std::vector<int>& passageirosIds, int destinoId);
    void avancarTempo(double horas);
    void finalizarEtapa(Viagem& v);

    // Consultas
    void listarCidades() const;
    void listarTrajetos() const;
    void listarTransportes() const;
    void listarPassageiros() const;
    void listarViagens() const;
    void localizarPassageiro(int id) const;
    void localizarTransporte(int id) const;
    void viagensEmAndamento() const;
    void cidadesMaisVisitadas() const;
    void melhorCaminho(int origemId, int destinoId, TipoTransporte tipo) const;

    // Persistência
    void salvar(const std::string& diretorio) const;
    void carregar(const std::string& diretorio);

    // Acesso para menu
    const std::vector<Cidade>&     getCidades()     const { return cidades; }
    const std::vector<Transporte>& getTransportes() const { return transportes; }
    const std::vector<Passageiro>& getPassageiros() const { return passageiros; }
};
