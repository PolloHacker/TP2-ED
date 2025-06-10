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

        int buscaVizinho(int id) const;

    public:
        Armazem();
        Armazem(int id);

        void adicionaVizinho(int vizinho);
        void removeVizinho(int id);
        Lista<int> getVizinhos() const;

        void armazenaPacote(const Pacote<std::string>& pacote, int idVizinho);
        Pacote<std::string> removePacotePorSecao(int idVizinho, int idPacote);

        int getId() const;
        void setId(int id);
};

#endif