#ifndef PILHA_HPP
#define PILHA_HPP

#include "node.hpp"
#include "lista.hpp"

class Pilha {
    public:
        Pilha();
        ~Pilha();

        void Empilha(int valor);
        int Desempilha();
        int Topo() const;
        bool Vazia() const;

    private:

        Node<int>* _topo;
};

#endif