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
    Lista<bool> visitados;
    Lista<int> caminho, antecessores;
    int n = this->vertices.GetVertices();

    for (int i = 0; i < n; i++) {
        Node<bool> node(false);
        visitados.InsereFim(node);

        Node<int> pred(-1);
        antecessores.InsereFim(pred);
    }
    
    visitados.Posiciona(v + 1)->SetData(true);
    fila.Enfileira(v);

    bool encontrou = false;
    while(!fila.Vazia() && !encontrou) {
        int currIdx = fila.Frente();
        fila.Desenfileira();

        Lista<int> vizinhos = this->vertices.GetVizinhos(currIdx);
        auto aux = vizinhos._head->GetNext();
        while (aux != nullptr) {
            int vizinho = aux->GetData();
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
        Node<int> curNode(curr);
        caminho.InsereInicio(curNode);
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

Grafo::~Grafo() {
    this->vertices.~ListaAdjacencia();
}