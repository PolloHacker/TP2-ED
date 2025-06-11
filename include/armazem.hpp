#ifndef ARMAZEM_HPP
#define ARMAZEM_HPP

#include <stdexcept>
#include <iostream>
#include "lista.hpp"
#include "vetor.hpp"
#include "pilha.hpp"
#include "pacote.hpp"

class Armazem {
    private:
        int _id;
        Lista<int> _vizinhos;
        Lista<Pilha<Pacote<std::string>>> _pacotesPorVizinho;
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

        void adicionaVizinho(int vizinho);
        void removeVizinho(int id);
        
        void armazenaPacote(const Pacote<std::string>& pacote, int idVizinho);
        Pacote<std::string> removePacotePorSecao(int idVizinho, int idPacote);
        
        void adicionaPacoteParaTransporte(int idVizinho, const Pacote<std::string>& pacote);
        Lista<int> getTransportesPorVizinho(int idVizinho);
        
        int getId() const;
        int getCooldown(int idVizinho);
        Lista<int> getVizinhos() const;

        void setId(int id);
        void setCooldown(int idVizinho, int cooldown);
        void setCapacidade(int idVizinho, int capacidade);
};

#endif