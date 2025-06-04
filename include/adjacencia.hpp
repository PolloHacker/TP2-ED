#ifndef ADJACENCIA_HPP
#define ADJACENCIA_HPP

#include "lista.hpp"

template <typename T>
class ListaAdjacencia {
    public:
        ListaAdjacencia();
        ~ListaAdjacencia();

        void InsereVertice();
        void InsereAresta(int v, int w);

        int GetVertices();
        int GetArestas();

        Lista<int> GetVizinhos(int v);

    private:
        Lista<Lista<T>> _lista;
        int _vertices;
        
};

#endif