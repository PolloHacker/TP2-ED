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
        Lista<Armazem> _vizinhos;
        Vetor<Pilha<Pacote<std::string>>> _pacotesPorVizinho;

        int buscaVizinho(int id) const;

    public:
        Armazem(int id);

        void adicionaVizinho(Armazem& vizinho);
        void removeVizinho(int id);
        Lista<Armazem> getVizinhos() const;

        void recebePacote(const Pacote<std::string>& pacote);
        Pacote<std::string> enviaPacote();
        Pilha<Pacote<std::string>> getPacotes() const;

        int getId() const;
        void setId(int id);
};

#endif