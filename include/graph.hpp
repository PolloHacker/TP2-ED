#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include "adjacencia.hpp"
#include "fila.hpp"
#include "heap.hpp"

class Grafo{
    private:
        ListaAdjacencia vertices;
    public:
        Grafo();
        void InsereVertice();
        void InsereAresta(int v, int w);
        int QuantidadeVertices();
        int QuantidadeArestas();
        int GrauMinimo();
        int GrauMaximo();
        void ImprimeVizinhos(int v);
        Lista<int> BFS(int v, int w);
        // Lista<int> Dykstra(int v, int w);
        // Lista<int> BellmanFord(int v, int w);
};

// Implementation

inline Grafo::Grafo(): vertices() {}

inline void Grafo::InsereVertice() {
    this->vertices.InsereVertice();
}

inline void Grafo::InsereAresta(int from, int to) {
    this->vertices.InsereAresta(from, to);
}

inline int Grafo::QuantidadeVertices() {
    return this->vertices.GetVertices();
}

inline int Grafo::QuantidadeArestas() {
    return this->vertices.GetArestas();
}

inline int Grafo::GrauMinimo() {
    int n = this->vertices.GetVertices();
    if (n == 0) return 0;
    int grauMinimo = this->vertices.GetVizinhos(1).GetTam();
    for (int i = 2; i <= n; i++) {
        int grauAtual = this->vertices.GetVizinhos(i).GetTam();
        if (grauAtual < grauMinimo) {
            grauMinimo = grauAtual;
        }
    }
    return grauMinimo;
}

inline int Grafo::GrauMaximo() {
    int n = this->vertices.GetVertices();
    if (n == 0) return 0;
    int grauMaximo = this->vertices.GetVizinhos(1).GetTam();
    for (int i = 2; i <= n; i++) {
        int grauAtual = this->vertices.GetVizinhos(i).GetTam();
        if (grauAtual > grauMaximo) {
            grauMaximo = grauAtual;
        }
    }
    return grauMaximo;
}

inline void Grafo::ImprimeVizinhos(int from) {
    int n = this->vertices.GetVertices();
    if (from < 1 || from > n) {
        std::cerr << "Índice de vértice inválido em ImprimeVizinhos: " << from << std::endl;
        return;
    }
    Lista<int> vizinhos = this->vertices.GetVizinhos(from);
    auto aux = vizinhos._head ? vizinhos._head->GetNext() : nullptr;
    while (aux != nullptr) {
        std::cout << aux->GetData() << " ";
        aux = aux->GetNext();
    }
    std::cout << std::endl;
}

inline Lista<int> Grafo::BFS(int v, int w) {
    int n = this->vertices.GetVertices();
    if (n == 0 || v < 1 || v > n || w < 1 || w > n) {
        std::cerr << "Índices de vértice inválidos em BFS: v=" << v << ", w=" << w << std::endl;
        return Lista<int>();
    }
    Fila<int> fila;
    Lista<bool> visitados;
    Lista<int> caminho, antecessores;
    for (int i = 1; i <= n; i++) {
        bool visitado = false;
        int antecessor = -1;
        visitados.InsereFim(visitado);
        antecessores.InsereFim(antecessor);
    }
    visitados.Posiciona(v)->SetData(true);
    fila.Enfileira(v);
    bool encontrou = false;
    while(!fila.Vazia() && !encontrou) {
        int currIdx = fila.Frente();
        fila.Desenfileira();
        Lista<int> vizinhos = this->vertices.GetVizinhos(currIdx);
        auto aux = vizinhos._head ? vizinhos._head->GetNext() : nullptr;
        while (aux != nullptr) {
            int vizinho = aux->GetData();
            if (vizinho < 1 || vizinho > n) {
                aux = aux->GetNext();
                continue;
            }
            if (!visitados.Posiciona(vizinho)->GetData()) {
                visitados.Posiciona(vizinho)->SetData(true);
                antecessores.Posiciona(vizinho)->SetData(currIdx);
                fila.Enfileira(vizinho);
                if (vizinho == w) {
                    encontrou = true;
                    break;
                }
            }
            aux = aux->GetNext();
        }
    }
    if (!visitados.Posiciona(w)->GetData()) {
        return Lista<int>();
    }
    int curr = w;
    while (curr != -1) {
        caminho.InsereInicio(curr);
        curr = antecessores.Posiciona(curr)->GetData();
    }
    return caminho;
}

#endif