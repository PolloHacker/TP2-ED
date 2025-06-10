#ifndef FILA_HPP
#define FILA_HPP

#include <stdexcept>
#include "node.hpp"

template <typename T>
class Fila {
    private:
        Node<T>* _inicio;
        Node<T>* _fim;
        
    public:
        Fila();
        Fila(const Fila<T>& other);
        Fila<T>& operator=(const Fila<T>& other);
        ~Fila();

        void Enfileira(T valor);
        T Desenfileira();
        T Frente() const;
        bool Vazia() const;

};

#include "fila.tpp"

#endif