#ifndef PILHA_HPP
#define PILHA_HPP

#include <stdexcept>
#include "node.hpp"
#include "lista.hpp"

template <typename T>
class Pilha {
    private:
        Node<T>* _topo;
        
    public:
        Pilha();
        ~Pilha();

        void Empilha(T valor);
        T Desempilha();
        T Topo() const;
        bool Vazia() const;

};

#endif