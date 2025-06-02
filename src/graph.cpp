#include <iostream>
#include "graph.hpp"

Grafo::Grafo(): vertices(ListaAdjacencia()) {}

void Grafo::InsereVertice() {
    this->vertices.InsereVertice();
}

void Grafo::InsereAresta(int from, int to) {
    this->vertices.InsereAresta(from, to);
}

int Grafo::QuantidadeVertices() {
    return this->vertices.GetVertices();
}

int Grafo::QuantidadeArestas() {
    return this->vertices.GetArestas();
}

int Grafo::GrauMinimo() {
    int grauMinimo = this->vertices.GetVizinhos(1).GetTam();

    for (int i = 1; i < this->vertices.GetVertices(); i++) {
        int grauAtual = this->vertices.GetVizinhos(i).GetTam();
        if (grauAtual < grauMinimo) {
            grauMinimo = grauAtual;
        }
    }
    return grauMinimo;
}

int Grafo::GrauMaximo() {
    int grauMaximo = this->vertices.GetVizinhos(1).GetTam();
    for (int i = 1; i < this->vertices.GetVertices(); i++) {
        int grauAtual = this->vertices.GetVizinhos(i).GetTam();
        if (grauAtual > grauMaximo) {
            grauMaximo = grauAtual;
        }
    }
    return grauMaximo;
}

void Grafo::ImprimeVizinhos(int from) {
    Lista<int> vizinhos = this->vertices.GetVizinhos(from);

    auto aux = vizinhos._head->GetNext();
    while (aux != nullptr) {
        std::cout << aux->GetData() << " ";
        aux = aux->GetNext();
    }
    std::cout << std::endl;
}

int Grafo::BFS(int v, int w) {
    // Implementação do algoritmo BFS
    Lista<bool> visitados;

    for (int i = 1; i <= this->vertices.GetVertices(); i++) {
        Node<bool> node(false);
        visitados.InsereFim(node);
    }
    


    return 0; // Placeholder
}

int Grafo::Dykstra(int v, int w) {
    // Implementação do algoritmo Dijkstra
    return 0; // Placeholder
}

int Grafo::BellmanFord(int v, int w) {
    // Implementação do algoritmo Bellman-Ford
    return 0; // Placeholder
}


Grafo::~Grafo() {
    this->vertices.~ListaAdjacencia();
}