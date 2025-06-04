#ifndef FILA_HPP
#define FILA_HPP

#include "node.hpp"
#include "lista.hpp"

template <typename T>
class Fila {
    public:
        Fila();
        ~Fila();

        void Enfileira(int valor);
        int Desenfileira();
        int Frente() const;
        bool Vazia() const;

    private:
        Node<T>* _inicio;
        Node<T>* _fim;
};

#endif