#ifndef ARMAZEM_HPP
#define ARMAZEM_HPP

#include <stdexcept>
#include <iomanip>
#include <iostream>
#include "lista.hpp"
#include "vetor.hpp"
#include "pilha.hpp"
#include "pacote.hpp"

class Armazem {
    private:
        int _id;
        Lista<int> _vizinhos;
        Lista<Pilha<int>> _pacotesPorVizinho;
        Lista<Lista<int>> _transportesPorVizinho;
        Lista<int> _cooldownsPorVizinho;
        Lista<int> _capacidadesPorVizinho;

        int buscaVizinho(int id) const;

    public:
        Armazem();
        Armazem(int id);
        
        // Copy constructor and assignment operator
        Armazem(const Armazem& other);
        Armazem& operator=(const Armazem& other);
        
        void adicionaVizinho(int idVizinho);
        void removeVizinho(int idVizinho);

        void armazenaPacote(int idVizinho, int idPacote);
        int removePacotePorSecao(int idVizinho, int idPacote);
        
        // Agora retorna os pacotes a serem rearmazenados
        Pilha<int> adicionaPacotesParaTransporte(int idVizinho, int& tempoAtual, int custoRemocao);
        void rearmazenarPacotes(int idVizinho, Pilha<int> pacotes, int tempoAtual);
        Lista<int> getTransportesPorVizinho(int idVizinho);

        bool temPacotesArmazenados();
        
        int getId() const;
        int getCooldown(int idVizinho);
        Lista<int> getVizinhos() const;

        void setId(int id);
        void setCooldown(int idVizinho, int cooldown);
        void setCapacidade(int idVizinho, int capacidade);
};

#endif