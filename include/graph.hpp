#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include "adjacencia.hpp"
#include "fila.hpp"

template <typename T>
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
        
        Lista<T> BFS(int v, int w);
        Lista<T> Dykstra(int v, int w);
        Lista<T> BellmanFord(int v, int w);

    private:
        
        ListaAdjacencia<T> vertices;
};

#endif