#ifndef FILA_HPP
#define FILA_HPP

#include "node.hpp"
#include "lista.hpp"

class Fila {
    public:
        Fila();
        ~Fila();

        void Enfileira(int valor);
        int Desenfileira();
        int Frente() const;
        bool Vazia() const;

    private:
        Node<int>* _inicio;
        Node<int>* _fim;
};

#endif