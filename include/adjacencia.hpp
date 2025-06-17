#ifndef ADJACENCIA_HPP
#define ADJACENCIA_HPP

#include <iostream>
#include "vetor.hpp"
#include "lista.hpp"

class ListaAdjacencia {
    private:
        Vetor<Lista<int>> _lista;
        int _vertices;

    public:
        ListaAdjacencia();

        void InsereVertice();
        void InsereAresta(int v, int w);

        int GetVertices();
        int GetArestas();

        Lista<int> GetVizinhos(int v);    
};

#endif