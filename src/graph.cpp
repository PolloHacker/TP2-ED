#include "graph.hpp"

template <typename T>
Grafo<T>::Grafo(): vertices(ListaAdjacencia()) {}

template <typename T>
void Grafo<T>::InsereVertice() {
    this->vertices.InsereVertice();
}

template <typename T>
void Grafo<T>::InsereAresta(int from, int to) {
    this->vertices.InsereAresta(from, to);
}

template <typename T>
int Grafo<T>::QuantidadeVertices() {
    return this->vertices.GetVertices();
}

template <typename T>
int Grafo<T>::QuantidadeArestas() {
    return this->vertices.GetArestas();
}

template <typename T>
int Grafo<T>::GrauMinimo() {
    int grauMinimo = this->vertices.GetVizinhos(1).GetTam();

    for (int i = 1; i < this->vertices.GetVertices(); i++) {
        int grauAtual = this->vertices.GetVizinhos(i).GetTam();
        if (grauAtual < grauMinimo) {
            grauMinimo = grauAtual;
        }
    }
    return grauMinimo;
}

template <typename T>
int Grafo<T>::GrauMaximo() {
    int grauMaximo = this->vertices.GetVizinhos(1).GetTam();
    for (int i = 1; i < this->vertices.GetVertices(); i++) {
        int grauAtual = this->vertices.GetVizinhos(i).GetTam();
        if (grauAtual > grauMaximo) {
            grauMaximo = grauAtual;
        }
    }
    return grauMaximo;
}

template <typename T>
void Grafo<T>::ImprimeVizinhos(int from) {
    Lista<int> vizinhos = this->vertices.GetVizinhos(from);

    auto aux = vizinhos._head->GetNext();
    while (aux != nullptr) {
        std::cout << aux->GetData() << " ";
        aux = aux->GetNext();
    }
    std::cout << std::endl;
}

template <typename T>
Lista<T> Grafo<T>::BFS(int v, int w) {
    // Implementação do algoritmo BFS
    Fila<int> fila;
    Lista<T> caminho;
    Lista<bool> visitados;

    for (int i = 1; i <= this->vertices.GetVertices(); i++) {
        Node<bool> node(false);
        visitados.InsereFim(node);
    }
    
    visitados.Posiciona(0)->SetData(true);
    fila.Enfileira(0);

    while(!fila.Vazia()) {
        int currIdx = fila.Frente();
        fila.Desenfileira();
        caminho.InsereFim(Node<T>(currIdx));

        Lista<T> vizinhos = this->vertices.GetVizinhos(currIdx);
        auto aux = vizinhos._head->GetNext();
        while (aux != nullptr) {
            int vizinho = aux->GetData();
            if (!visitados.Posiciona(vizinho)->GetData()) {
                visitados.Posiciona(vizinho)->SetData(true);
                fila.Enfileira(vizinho);
            }
            aux = aux->GetNext();
        }
    }

    return caminho;
}

template <typename T>
Lista<T> Grafo<T>::Dykstra(int v, int w) {
    // Implementação do algoritmo Dijkstra
    return 0; // Placeholder
}

template <typename T>
Lista<T> Grafo<T>::BellmanFord(int v, int w) {
    // Implementação do algoritmo Bellman-Ford
    return 0; // Placeholder
}

template <typename T>
Grafo<T>::~Grafo() {
    this->vertices.~ListaAdjacencia();
}