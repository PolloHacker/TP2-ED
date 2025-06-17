#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include "vetor.hpp"
#include "lista.hpp"
#include "fila.hpp"
#include "heap.hpp"

class Grafo {
private:
    Vetor<Lista<int>> _lista;
    int _vertices;

public:
    Grafo();

    void InsereVertice();
    void InsereAresta(int v, int w);

    int QuantidadeVertices();
    int QuantidadeArestas();

    int GrauMinimo();
    int GrauMaximo();

    void ImprimeVizinhos(int v);
    Lista<int> GetVizinhos(int v);
    Lista<int> BFS(int v, int w);
    // Lista<int> Dykstra(int v, int w);
    // Lista<int> BellmanFord(int v, int w);
};

#endif