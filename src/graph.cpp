#include "graph.hpp"

Grafo::Grafo() : _lista(), _vertices(0) {}

void Grafo::InsereVertice() {
    Lista<int> novaLista;
    this->_lista.insereFim(novaLista);
    this->_vertices++;
}

void Grafo::InsereAresta(int from, int to) {
    Lista<int> vizinhos = this->_lista[from - 1];
    vizinhos.InsereFim(to);
    this->_lista[from - 1] = vizinhos;
}

int Grafo::QuantidadeVertices() {
    return this->_vertices;
}

int Grafo::QuantidadeArestas() {
    int count = 0;
    Node<int>* current;
    for (int i = 1; i <= this->_vertices; i++) {
        Lista<int> vizinhos = this->_lista[i - 1];
        current = vizinhos._head->GetNext();
        while (current != nullptr) {
            if (current->GetData() + 1 > i) {
                count++;
            }
            current = current->GetNext();
        }
    }
    return count;
}

int Grafo::GrauMinimo() {
    int n = this->_vertices;
    if (n == 0) return 0;
    int grauMinimo = this->GetVizinhos(1).GetTam();
    for (int i = 2; i <= n; i++) {
        int grauAtual = this->GetVizinhos(i).GetTam();
        if (grauAtual < grauMinimo) {
            grauMinimo = grauAtual;
        }
    }
    return grauMinimo;
}

int Grafo::GrauMaximo() {
    int n = this->_vertices;
    if (n == 0) return 0;
    int grauMaximo = this->GetVizinhos(1).GetTam();
    for (int i = 2; i <= n; i++) {
        int grauAtual = this->GetVizinhos(i).GetTam();
        if (grauAtual > grauMaximo) {
            grauMaximo = grauAtual;
        }
    }
    return grauMaximo;
}

void Grafo::ImprimeVizinhos(int from) {
    int n = this->_vertices;
    if (from < 1 || from > n) {
        std::cerr << "Índice de vértice inválido em ImprimeVizinhos: " << from << std::endl;
        return;
    }
    Lista<int> vizinhos = this->GetVizinhos(from);
    auto aux = vizinhos._head ? vizinhos._head->GetNext() : nullptr;
    while (aux != nullptr) {
        std::cout << aux->GetData() << " ";
        aux = aux->GetNext();
    }
    std::cout << std::endl;
}

Lista<int> Grafo::GetVizinhos(int v) {
    if (v < 1 || v > this->_vertices) {
        throw std::out_of_range("Índice de vértice inválido.");
    }
    return this->_lista[v - 1];
}

Lista<int> Grafo::BFS(int v, int w) {
    int n = this->_vertices;
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
        Lista<int> vizinhos = this->GetVizinhos(currIdx);
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

// Lista<int> Grafo::Dykstra(int v, int w) {
//     // Implementação do algoritmo Dijkstra
//     return Lista<int>(); // Placeholder
// }

// Lista<int> Grafo::BellmanFord(int v, int w) {
//     // Implementação do algoritmo Bellman-Ford
//     return Lista<int>(); // Placeholder
// }