#ifndef PILHA_HPP
#define PILHA_HPP

#include <stdexcept>
#include "node.hpp"

template <typename T>
class Pilha {
    private:
        Node<T>* _topo;
        
    public:
        Pilha();
        Pilha(const Pilha<T>& other);
        Pilha<T>& operator=(const Pilha<T>& other);
        ~Pilha();

        void Empilha(T valor);
        T Desempilha();
        T Topo() const;
        bool Vazia() const;

};

#include "pilha.tpp"

#endif