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

Lista<int> Grafo::BFS(int v, int w) {
    // Implementação do algoritmo BFS
    Fila<int> fila;
    Lista<int> caminho;
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
        caminho.InsereFim(Node<int>(currIdx));

        Lista<int> vizinhos = this->vertices.GetVizinhos(currIdx);
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

Lista<int> Grafo::Dykstra(int v, int w) {
    // Implementação do algoritmo Dijkstra
        
    return Lista<int>(); // Placeholder
}

Lista<int> Grafo::BellmanFord(int v, int w) {
    // Implementação do algoritmo Bellman-Ford
    return Lista<int>(); // Placeholder
}

Grafo::~Grafo() {
    this->vertices.~ListaAdjacencia();
}