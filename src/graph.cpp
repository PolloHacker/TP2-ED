#include "graph.hpp"

Grafo::Grafo(): vertices() {}

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
    int n = this->vertices.GetVertices();
    if (n == 0) return 0;
    int grauMinimo = this->vertices.GetVizinhos(1).GetTam();

    for (int i = 2; i <= n; i++) { // fix: start at 2, go to n inclusive
        int grauAtual = this->vertices.GetVizinhos(i).GetTam();
        if (grauAtual < grauMinimo) {
            grauMinimo = grauAtual;
        }
    }
    return grauMinimo;
}

int Grafo::GrauMaximo() {
    int n = this->vertices.GetVertices();
    if (n == 0) return 0;
    int grauMaximo = this->vertices.GetVizinhos(1).GetTam();
    for (int i = 2; i <= n; i++) { // fix: start at 2, go to n inclusive
        int grauAtual = this->vertices.GetVizinhos(i).GetTam();
        if (grauAtual > grauMaximo) {
            grauMaximo = grauAtual;
        }
    }
    return grauMaximo;
}

void Grafo::ImprimeVizinhos(int from) {
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

Lista<int> Grafo::BFS(int v, int w) {
    int n = this->vertices.GetVertices();
    if (n == 0 || v < 0 || v >= n || w < 0 || w >= n) {
        std::cerr << "Índices de vértice inválidos em BFS: v=" << v << ", w=" << w << std::endl;
        return Lista<int>();
    }
    Fila<int> fila;
    Lista<bool> visitados;
    Lista<int> caminho, antecessores;

    for (int i = 0; i < n; i++) {
        bool visitado = false;
        visitados.InsereFim(visitado);

        int pred = -1;
        antecessores.InsereFim(pred);
    }

    visitados.Posiciona(v + 1)->SetData(true);
    fila.Enfileira(v);

    bool encontrou = false;
    while(!fila.Vazia() && !encontrou) {
        int currIdx = fila.Frente();
        fila.Desenfileira();

        Lista<int> vizinhos = this->vertices.GetVizinhos(currIdx + 1); // fix: +1 for 1-based
        auto aux = vizinhos._head ? vizinhos._head->GetNext() : nullptr;
        while (aux != nullptr) {
            int vizinho = aux->GetData();
            if (vizinho < 0 || vizinho >= n) {
                aux = aux->GetNext();
                continue;
            }
            if (!visitados.Posiciona(vizinho + 1)->GetData()) {
                visitados.Posiciona(vizinho + 1)->SetData(true);
                antecessores.Posiciona(vizinho + 1)->SetData(currIdx);
                fila.Enfileira(vizinho);

                if (vizinho == w) {
                    encontrou = true;
                    break;
                }
            }
            aux = aux->GetNext();
        }
    }

    if (!visitados.Posiciona(w + 1)->GetData()) {
        return Lista<int>();
    }
    int curr = w;
    while (curr != -1) {
        caminho.InsereInicio(curr);
        curr = antecessores.Posiciona(curr + 1)->GetData();
    }

    return caminho;
}

// Lista<int> Grafo::Dykstra(int v, int w) {
//     // Implementação do algoritmo Dijkstra
//     return Lista<int>(); // Placeholder
// }

// Lista<int> Grafo::BellmanFord(int v, int w) {
//     // Implementação do algoritmo Bellman-Ford
//     return Lista<int>(); // Placeholder
// }