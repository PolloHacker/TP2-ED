#ifndef ARMAZEM_HPP
#define ARMAZEM_HPP

#include <stdexcept>
#include <iomanip>
#include <iostream>
#include "lista.hpp"
#include "vetor.hpp"
#include "pilha.hpp"
#include "hashTable.hpp"
#include "pacote.hpp"

class Armazem {
    private:
        int _id;
        Lista<int> _vizinhos;

        HashTable<Pilha<int>> _pacotesPorVizinho;
        HashTable<Lista<int>> _transportesPorVizinho;
        HashTable<int> _cooldownsPorVizinho;
        HashTable<int> _capacidadesPorVizinho;

        // Vetor<Pilha<int>> _pacotesPorVizinho;

        // Vetor<Lista<int>> _transportesPorVizinho;
        // Vetor<int> _cooldownsPorVizinho;
        // Vetor<int> _capacidadesPorVizinho;

    public:
        Armazem();
        Armazem(int id);
        
        // Copy constructor and assignment operator
        Armazem(const Armazem& other);
        Armazem& operator=(const Armazem& other);
        
        void adicionaVizinho(int idVizinho);
        void armazenaPacote(int idVizinho, int idPacote, Metricas& metricas);
        int removePacotePorSecao(int idVizinho, int idPacote);
        
        // Agora retorna os pacotes a serem rearmazenados
        Pilha<int> adicionaPacotesParaTransporte(int idVizinho, int& tempoAtual, int custoRemocao, Metricas& metricas);
        void rearmazenarPacotes(int idVizinho, Pilha<int> pacotes, int tempoAtual, Metricas& metricas);
        Lista<int> getTransportesPorVizinho(int idVizinho);
        
        int getId() const;
        int getCooldown(int idVizinho);
        Lista<int> getVizinhos() const;

        void setId(int id);
        void setCooldown(int idVizinho, int cooldown);
        void setCapacidade(int idVizinho, int capacidade);
};

#endif