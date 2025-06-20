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

/**
 * @brief Construtor padrão da classe Grafo.
 *
 * Inicializa um objeto Grafo com zero vértices.
 */
Grafo::Grafo(): _vertices(0) {}

/**
 * @brief Insere um novo vértice no grafo.
 *
 * Este método incrementa o contador de vértices e adiciona uma nova lista
 * de adjacências para o novo vértice.
 */
void Grafo::InsereVertice() {
    this->_vertices++;
    Lista<int> novaLista;
    this->_lista.insereFim(novaLista);
}

/**
 * @brief Insere uma aresta entre dois vértices no grafo.
 *
 * Este método adiciona uma aresta entre os vértices 'from' e 'to'.
 * Se os índices dos vértices forem inválidos, uma mensagem de erro é exibida.
 *
 * @param from Índice do vértice de origem (1 a n).
 * @param to Índice do vértice de destino (1 a n).
 */
void Grafo::InsereAresta(int from, int to) {
    if (from < 1 || from > this->_vertices || to < 1 || to > this->_vertices) {
        std::cerr << "Índices de vértice inválidos: from=" << from << ", to=" << to << std::endl;
        return;
    }
    this->_lista[from - 1].InsereFim(to);
    this->_lista[to - 1].InsereFim(from);
}

/**
 * @brief Retorna a quantidade de vértices no grafo.
 *
 * @return int Número de vértices no grafo.
 */
int Grafo::QuantidadeVertices() {
    return this->_vertices;
}

/**
 * @brief Retorna a quantidade de arestas no grafo.

 *
 * @return int Número de arestas no grafo.
 */
int Grafo::QuantidadeArestas() {
    int arestas = 0;

    // Percorre a lista de adjacências para contar as arestas
    for (int i = 0; i < this->_vertices; i++) {
        arestas += this->_lista[i].GetTam();
    }
    return arestas / 2; // Cada aresta é contada duas vezes
}

/**
 * @brief Retorna a lista de vizinhos de um vértice específico.
 *
 * @param v Índice do vértice (1 a n).
 * @return Lista<int> Lista de vizinhos do vértice especificado.
 */
Lista<int> Grafo::GetVizinhos(int v) {
    if (v < 1 || v > this->_vertices) {
        std::cerr << "Índice de vértice inválido: " << v << std::endl;
        return Lista<int>();
    }
    return this->_lista[v - 1];
}

/**
 * @brief Retorna o grau mínimo de todos os vértices do grafo.
 *
 * O grau de um vértice é o número de arestas conectadas a ele.
 * Se o grafo não tiver vértices, retorna 0.
 *
 * @return int Grau mínimo entre os vértices do grafo.
 */
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

/**
 * @brief Retorna o grau máximo de todos os vértices do grafo.
 *
 * O grau de um vértice é o número de arestas conectadas a ele.
 * Se o grafo não tiver vértices, retorna 0.
 *
 * @return int Grau máximo entre os vértices do grafo.
 */
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

/**
 * @brief Imprime os vizinhos de um vértice específico.
 *
 * Este método imprime os índices dos vizinhos do vértice 'from'.
 * Se o índice do vértice for inválido, uma mensagem de erro é exibida.
 *
 * @param from Índice do vértice (1 a n).
 */
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

/**
 * @brief Realiza uma busca em largura (BFS) entre dois vértices no grafo.
 *
 * Este método encontra o caminho mais curto entre os vértices 'v' e 'w'
 * usando o algoritmo BFS. Se não houver caminho, retorna uma lista vazia.
 *
 * @param v Índice do vértice de origem (1 a n).
 * @param w Índice do vértice de destino (1 a n).
 * @return Lista<int> Lista de vértices no caminho encontrado, ou vazia se não houver caminho.
 */
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
        
        // Percorre a lista de vizinhos do vértice atual
        while (aux != nullptr) {
            int vizinho = aux->GetData();
            // Verifica se o vizinho está dentro dos limites do grafo
            if (vizinho < 1 || vizinho > n) {
                aux = aux->GetNext();
                continue;
            }
            // Se o vizinho ainda não foi visitado, marca como visitado e adiciona à fila
            if (!visitados.Posiciona(vizinho)->GetData()) {
                visitados.Posiciona(vizinho)->SetData(true);
                antecessores.Posiciona(vizinho)->SetData(currIdx);
                fila.Enfileira(vizinho);

                // Se o vizinho é o destino, marca que encontrou o caminho
                if (vizinho == w) {
                    encontrou = true;
                    break;
                }
            }
            aux = aux->GetNext();
        }
    }

    // Se não encontrou o caminho, retorna uma lista vazia
    if (!visitados.Posiciona(w)->GetData()) {
        return Lista<int>();
    }

    int curr = w;
    // Reconstrói o caminho a partir dos antecessores
    while (curr != -1) {
        caminho.InsereInicio(curr);
        curr = antecessores.Posiciona(curr)->GetData();
    }

    return caminho;
}

#endif