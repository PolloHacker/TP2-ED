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
    // Lista<int> Dykstra(int v, int w);    // Lista<int> BellmanFord(int v, int w);
};

Grafo::Grafo(): _vertices(0) {}

void Grafo::InsereVertice() {
    this->_vertices++;
    Lista<int> novaLista;
    this->_lista.insereFim(novaLista);
}

void Grafo::InsereAresta(int from, int to) {
    if (from < 1 || from > this->_vertices || to < 1 || to > this->_vertices) {
        std::cerr << "Índices de vértice inválidos: from=" << from << ", to=" << to << std::endl;
        return;
    }
    this->_lista[from - 1].InsereFim(to);
    this->_lista[to - 1].InsereFim(from);
}

int Grafo::QuantidadeVertices() {
    return this->_vertices;
}

int Grafo::QuantidadeArestas() {
    int arestas = 0;
    for (int i = 0; i < this->_vertices; i++) {
        arestas += this->_lista[i].GetTam();
    }
    return arestas / 2; // Cada aresta é contada duas vezes
}

Lista<int> Grafo::GetVizinhos(int v) {
    if (v < 1 || v > this->_vertices) {
        std::cerr << "Índice de vértice inválido: " << v << std::endl;
        return Lista<int>();
    }
    return this->_lista[v - 1];
}

int Grafo::GrauMinimo() {
    if (this->_vertices == 0) return 0;
    int grauMinimo = this->_lista[0].GetTam();

    for (int i = 1; i < this->_vertices; i++) {
        int grauAtual = this->_lista[i].GetTam();
        if (grauAtual < grauMinimo) {
            grauMinimo = grauAtual;
        }
    }
    return grauMinimo;
}

int Grafo::GrauMaximo() {
    if (this->_vertices == 0) return 0;
    int grauMaximo = this->_lista[0].GetTam();
    for (int i = 1; i < this->_vertices; i++) {
        int grauAtual = this->_lista[i].GetTam();
        if (grauAtual > grauMaximo) {
            grauMaximo = grauAtual;
        }
    }
    return grauMaximo;
}

void Grafo::ImprimeVizinhos(int from) {
    if (from < 1 || from > this->_vertices) {
        std::cerr << "Índice de vértice inválido em ImprimeVizinhos: " << from << std::endl;
        return;
    }
    Lista<int> vizinhos = this->_lista[from - 1];

    auto aux = vizinhos._head ? vizinhos._head->GetNext() : nullptr;
    while (aux != nullptr) {
        std::cout << aux->GetData() << " ";
        aux = aux->GetNext();
    }
    std::cout << std::endl;
}

Lista<int> Grafo::BFS(int v, int w) {
    int n = this->_vertices;
    // Ajuste para índices de vértices de 1 a n
    if (n == 0 || v < 1 || v > n || w < 1 || w > n) {
        std::cerr << "Índices de vértice inválidos em BFS: v=" << v << ", w=" << w << std::endl;
        return Lista<int>();
    }
    Fila<int> fila;
    Lista<bool> visitados;
    Lista<int> caminho, antecessores;

    // Inicializa listas para 1 a n (posição 1 corresponde ao vértice 1)
    for (int i = 1; i <= n; i++) {
        bool visitado = false;
        int antecessor = -1; // -1 indica que não há antecessor
        visitados.InsereFim(visitado);
        antecessores.InsereFim(antecessor);
    }

    visitados.Posiciona(v)->SetData(true);
    fila.Enfileira(v);

    bool encontrou = false;
    while(!fila.Vazia() && !encontrou) {
        int currIdx = fila.Frente();
        fila.Desenfileira();

        Lista<int> vizinhos = this->_lista[currIdx - 1];
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