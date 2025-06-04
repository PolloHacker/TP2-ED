#ifndef PILHA_HPP
#define PILHA_HPP

#include "node.hpp"
#include "lista.hpp"

template <typename T>
class Pilha {
    public:
        Pilha();
        ~Pilha();

        void Empilha(T valor);
        T Desempilha();
        T Topo() const;
        bool Vazia() const;

    private:

        Node<T>* _topo;
};

#endif