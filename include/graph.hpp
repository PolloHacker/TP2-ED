#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include "vetor.hpp"
#include "lista.hpp"
#include "fila.hpp"
#include "heap.hpp"

// Forward declarations
class Armazem;
struct Vizinho;

// Simple structure to represent a weighted edge
struct Edge {
    int destino;
    int peso;
    
    Edge() : destino(0), peso(0) {}
    Edge(int dest, int weight) : destino(dest), peso(weight) {}
};

class Grafo {
private:
    Vetor<Lista<Edge>> _lista;  // Unified weighted adjacency list
    int _vertices;

public:
    Grafo();

    void InsereVertice();
    void InsereAresta(int v, int w);                    // Now adds edges with weight 1
    void InsereAresta(int v, int w, int peso);          // Overloaded for weighted edges

    int QuantidadeVertices();
    int QuantidadeArestas();

    int GrauMinimo();
    int GrauMaximo();

    void ImprimeVizinhos(int v);
    Lista<int> GetVizinhos(int v);                      // Returns just the vertices
    Lista<Edge> GetVizinhosWeighted(int v);             // Returns vertices with weights
    Lista<int> BFS(int v, int w);
    Lista<int> Dykstra(int v, int w);
    Lista<int> DijkstraDinamico(int origem, int destino, const Vetor<Armazem>& armazens);  // Dynamic pathfinding
    Lista<int> DijkstraComCongestion(int origem, int destino, const Vetor<Vetor<int>>& congestion_matrix);  // Alternative approach
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
    Lista<Edge> novaLista;
    this->_lista.insereFim(novaLista);
}

/**
 * @brief Insere uma aresta entre dois vértices no grafo (peso padrão = 1).
 *
 * Este método adiciona uma aresta entre os vértices 'from' e 'to' com peso 1.
 * Se os índices dos vértices forem inválidos, uma mensagem de erro é exibida.
 *
 * @param from Índice do vértice de origem (1 a n).
 * @param to Índice do vértice de destino (1 a n).
 */
void Grafo::InsereAresta(int from, int to) {
    InsereAresta(from, to, 1);  // Delega para a versão com peso, usando peso 1
}

/**
 * @brief Insere uma aresta com peso entre dois vértices no grafo.
 *
 * Este método adiciona uma aresta com peso entre os vértices 'from' e 'to'.
 * Para grafos dirigidos, adiciona apenas from -> to.
 * Para grafos não-dirigidos, adiciona tanto from -> to quanto to -> from.
 *
 * @param from Índice do vértice de origem (1 a n).
 * @param to Índice do vértice de destino (1 a n).
 * @param peso Peso da aresta.
 */
void Grafo::InsereAresta(int from, int to, int peso) {
    if (from < 1 || from > this->_vertices || to < 1 || to > this->_vertices) {
        std::cerr << "Índices de vértice inválidos: from=" << from << ", to=" << to << std::endl;
        return;
    }
    Edge edge(to, peso);
    this->_lista[from - 1].InsereFim(edge);
    
    // For undirected graphs, add the reverse edge as well
    // Comment out the next two lines if you want a directed graph
    Edge edgeReverse(from, peso);
    this->_lista[to - 1].InsereFim(edgeReverse);
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
 * @brief Retorna a lista de vizinhos de um vértice específico (apenas IDs).
 *
 * @param v Índice do vértice (1 a n).
 * @return Lista<int> Lista de IDs dos vizinhos do vértice especificado.
 */
Lista<int> Grafo::GetVizinhos(int v) {
    if (v < 1 || v > this->_vertices) {
        std::cerr << "Índice de vértice inválido: " << v << std::endl;
        return Lista<int>();
    }
    
    Lista<int> vizinhos;
    Lista<Edge> arestas = this->_lista[v - 1];
    auto aux = arestas._head ? arestas._head->GetNext() : nullptr;
    
    while (aux != nullptr) {
        int vizinho = aux->GetData().destino;
        vizinhos.InsereFim(vizinho);
        aux = aux->GetNext();
    }
    
    return vizinhos;
}

/**
 * @brief Retorna a lista de vizinhos com pesos de um vértice específico.
 *
 * @param v Índice do vértice (1 a n).
 * @return Lista<Edge> Lista de arestas do vértice especificado.
 */
Lista<Edge> Grafo::GetVizinhosWeighted(int v) {
    if (v < 1 || v > this->_vertices) {
        std::cerr << "Índice de vértice inválido: " << v << std::endl;
        return Lista<Edge>();
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
    Lista<Edge> vizinhos = this->_lista[from - 1];

    auto aux = vizinhos._head ? vizinhos._head->GetNext() : nullptr;
    while (aux != nullptr) {
        std::cout << aux->GetData().destino << " ";
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

        Lista<Edge> vizinhos = this->_lista[currIdx - 1];
        auto aux = vizinhos._head ? vizinhos._head->GetNext() : nullptr;
        
        // Percorre a lista de vizinhos do vértice atual
        while (aux != nullptr) {
            int vizinho = aux->GetData().destino;
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

/**
 * @brief Implementa o algoritmo de Dijkstra para encontrar o caminho mais curto em um grafo ponderado.
 *
 * Este método encontra o caminho de menor custo entre os vértices 'v' e 'w'
 * usando o algoritmo de Dijkstra. Se não houver caminho, retorna uma lista vazia.
 * O algoritmo funciona apenas com pesos não-negativos.
 *
 * @param v Índice do vértice de origem (1 a n).
 * @param w Índice do vértice de destino (1 a n).
 * @return Lista<int> Lista de vértices no caminho de menor custo, ou vazia se não houver caminho.
 */
Lista<int> Grafo::Dykstra(int v, int w) {
    int n = this->_vertices;
    const int INF = 2147483647; // Valor máximo para representar infinito
    
    // Validação de entrada
    if (n == 0 || v < 1 || v > n || w < 1 || w > n) {
        std::cerr << "Índices de vértice inválidos em Dijkstra: v=" << v << ", w=" << w << std::endl;
        return Lista<int>();
    }
    
    // Arrays para distâncias e antecessores (índices de 1 a n)
    Vetor<int> distancia(n + 1);
    Vetor<int> antecessor(n + 1);
    Vetor<bool> visitado(n + 1);
    
    // Inicialização
    for (int i = 1; i <= n; i++) {
        distancia[i] = INF;
        antecessor[i] = -1;
        visitado[i] = false;
    }
    
    distancia[v] = 0;
    
    // Algoritmo de Dijkstra
    for (int count = 0; count < n; count++) {
        // Encontra o vértice não visitado com menor distância
        int u = -1;
        int minDist = INF;
        
        for (int i = 1; i <= n; i++) {
            if (!visitado[i] && distancia[i] < minDist) {
                minDist = distancia[i];
                u = i;
            }
        }
        
        // Se não encontrou vértice válido, todos os restantes são inacessíveis
        if (u == -1) break;
        
        visitado[u] = true;
        
        // Se chegou ao destino, pode parar
        if (u == w) break;
        
        // Atualiza as distâncias dos vizinhos
        Lista<Edge> vizinhos = this->_lista[u - 1];
        auto aux = vizinhos._head ? vizinhos._head->GetNext() : nullptr;
        
        while (aux != nullptr) {
            Edge edge = aux->GetData();
            int vizinho = edge.destino;
            int peso = edge.peso;
            
            // Verifica se o vizinho está dentro dos limites
            if (vizinho >= 1 && vizinho <= n && !visitado[vizinho]) {
                int novaDist = distancia[u] + peso;
                
                // Relaxamento da aresta
                if (novaDist < distancia[vizinho]) {
                    distancia[vizinho] = novaDist;
                    antecessor[vizinho] = u;
                }
            }
            aux = aux->GetNext();
        }
    }
    
    // Se não há caminho para o destino
    if (distancia[w] == INF) {
        return Lista<int>();
    }
    
    // Reconstrói o caminho a partir dos antecessores
    Lista<int> caminho;
    int curr = w;
    
    while (curr != -1) {
        caminho.InsereInicio(curr);
        curr = antecessor[curr];
    }
    
    return caminho;
}

/**
 * @brief Implementa o algoritmo de Dijkstra dinâmico com consciência de congestão.
 *
 * Este método encontra o caminho de menor custo considerando tanto os pesos estáticos
 * das arestas quanto a congestão atual dos armazéns. A implementação real está
 * delegada para a classe Transporte para evitar dependências circulares.
 *
 * @param origem Índice do vértice de origem (1 a n).
 * @param destino Índice do vértice de destino (1 a n).
 * @param armazens Vetor contendo os dados atuais dos armazéns.
 * @return Lista<int> Lista de vértices no caminho de menor custo dinâmico, ou vazia se não houver caminho.
 */
Lista<int> Grafo::DijkstraDinamico(int origem, int destino, const Vetor<Armazem>& armazens) {
    // A implementação real com consulta de congestão é feita na classe Transporte
    // Aqui retornamos o Dijkstra estático para evitar dependências circulares
    (void)armazens; // Suprime warning de parâmetro não usado
    return this->Dykstra(origem, destino);
}

/**
 * @brief Algoritmo de Dijkstra com matriz de congestão.
 *
 * Este método implementa Dijkstra considerando custos de congestão através de uma matriz
 * onde congestion_matrix[i][j] representa o custo adicional de ir do vértice i para j.
 *
 * @param origem Índice do vértice de origem (1 a n).
 * @param destino Índice do vértice de destino (1 a n).
 * @param congestion_matrix Matriz de custos de congestão adicionais.
 * @return Lista<int> Lista de vértices no caminho de menor custo, ou vazia se não houver caminho.
 */
Lista<int> Grafo::DijkstraComCongestion(int origem, int destino, const Vetor<Vetor<int>>& congestion_matrix) {
    int n = this->_vertices;
    const int INF = 2147483647; // Valor máximo para representar infinito
    
    // Validação de entrada
    if (n == 0 || origem < 1 || origem > n || destino < 1 || destino > n) {
        std::cerr << "Índices de vértice inválidos em DijkstraComCongestion: origem=" << origem << ", destino=" << destino << std::endl;
        return Lista<int>();
    }
    
    // Arrays para distâncias e antecessores (índices de 1 a n)
    Vetor<int> distancia(n + 1);
    Vetor<int> antecessor(n + 1);
    Vetor<bool> visitado(n + 1);
    
    // Inicialização
    for (int i = 1; i <= n; i++) {
        distancia[i] = INF;
        antecessor[i] = -1;
        visitado[i] = false;
    }
    
    distancia[origem] = 0;
    
    // Algoritmo de Dijkstra com custos de congestão
    for (int count = 0; count < n; count++) {
        // Encontra o vértice não visitado com menor distância
        int u = -1;
        int minDist = INF;
        
        for (int i = 1; i <= n; i++) {
            if (!visitado[i] && distancia[i] < minDist) {
                minDist = distancia[i];
                u = i;
            }
        }
        
        // Se não encontrou vértice válido, todos os restantes são inacessíveis
        if (u == -1) break;
        
        visitado[u] = true;
        
        // Se chegou ao destino, pode parar
        if (u == destino) break;
        
        // Atualiza as distâncias dos vizinhos com custos dinâmicos
        Lista<Edge> vizinhos = this->_lista[u - 1];
        auto aux = vizinhos._head ? vizinhos._head->GetNext() : nullptr;
        
        while (aux != nullptr) {
            Edge edge = aux->GetData();
            int vizinho = edge.destino;
            int peso_estatico = edge.peso;
            
            // Verifica se o vizinho está dentro dos limites
            if (vizinho >= 1 && vizinho <= n && !visitado[vizinho]) {
                // Calcula o custo dinâmico baseado na matriz de congestão
                int custo_congestion = 0;
                
                // Acessa a matriz de congestão se disponível
                if (u <= congestion_matrix.getSize() && vizinho <= congestion_matrix[u - 1].getSize()) {
                    try {
                        custo_congestion = congestion_matrix[u - 1][vizinho - 1];
                    } catch (const std::exception&) {
                        custo_congestion = 0;
                    }
                }
                
                int peso_dinamico = peso_estatico + custo_congestion;
                int novaDist = distancia[u] + peso_dinamico;
                
                // Relaxamento da aresta com peso dinâmico
                if (novaDist < distancia[vizinho]) {
                    distancia[vizinho] = novaDist;
                    antecessor[vizinho] = u;
                }
            }
            aux = aux->GetNext();
        }
    }
    
    // Se não há caminho para o destino
    if (distancia[destino] == INF) {
        return Lista<int>();
    }
    
    // Reconstrói o caminho a partir dos antecessores
    Lista<int> caminho;
    int curr = destino;
    
    while (curr != -1) {
        caminho.InsereInicio(curr);
        curr = antecessor[curr];
    }
    
    return caminho;
}

#endif