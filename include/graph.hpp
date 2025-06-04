#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include "adjacencia.hpp"
#include "fila.hpp"


class Grafo{
    public:
        Grafo();
        ~Grafo();

        void InsereVertice();
        void InsereAresta(int v, int w);

        int QuantidadeVertices();
        int QuantidadeArestas();

        int GrauMinimo();
        int GrauMaximo();

        void ImprimeVizinhos(int v);
        
        Lista<int> BFS(int v, int w);
        int Dykstra(int v, int w);
        int BellmanFord(int v, int w);

    private:
        
        ListaAdjacencia vertices;
};

#endif