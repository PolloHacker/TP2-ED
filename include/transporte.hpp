#ifndef TRANSPORTE_HPP
#define TRANSPORTE_HPP

#include "graph.hpp"
#include "armazem.hpp"

class Transporte {
    private:
        Grafo _rede;

    public:
        Transporte();

        Armazem adicionaArmazem(int id);
        void conectaArmazens(int id1, int id2, int peso);
        
        void imprimeRede();

        Lista<int> calculaRota(int origem, int destino);
        Lista<int> calculaRotaComPeso(int origem, int destino);
        Lista<int> calculaRotaDinamica(int origem, int destino, const Vetor<Armazem>& armazens);
        void decairCongestionTodosArmazens(Vetor<Armazem>& armazens);
        Grafo getRede() const;
};


/**
 * @brief Construtor padrão da classe Transporte.
 *
 * Inicializa o objeto Transporte criando uma nova instância de Grafo para a rede interna.
 */
Transporte::Transporte() : _rede(Grafo()) {}

/**
 * @brief Adiciona um novo armazém à rede de transporte.
 *
 * Este método insere um novo vértice no grafo e retorna um objeto Armazem
 * com o ID fornecido.
 *
 * @param id Identificador único do armazém a ser adicionado.
 * @return Armazem O novo armazém criado com o ID especificado.
 */
Armazem Transporte::adicionaArmazem(int id) {
    this->_rede.InsereVertice();
    
    return Armazem(id);
}

/**
 * @brief Conecta dois armazéns na rede de transporte.
 *
 * Este método insere uma aresta entre os armazéns com os IDs especificados,
 * permitindo que eles se comuniquem na rede.
 *
 * @param id1 Identificador do primeiro armazém.
 * @param id2 Identificador do segundo armazém.
 * @param peso Peso da aresta que conecta os armazéns.
 */
void Transporte::conectaArmazens(int id1, int id2, int peso) {
    this->_rede.InsereAresta(id1, id2, peso);
}

/**
 * @brief Imprime a rede de armazéns e suas conexões.
 *
 * Este método percorre todos os armazéns na rede e imprime seus vizinhos,
 * mostrando as conexões entre eles.
 */
void Transporte::imprimeRede() {
    for (int i = 1; i <= this->_rede.QuantidadeVertices(); ++i) {
        std::cout << "Armazém " << i << ": ";
        this->_rede.ImprimeVizinhos(i);
    }
}

/**
 * @brief Calcula a rota entre dois armazéns na rede de transporte.
 *
 * Este método utiliza o algoritmo de busca em largura (BFS) para encontrar
 * o caminho mais curto entre os armazéns com os IDs de origem e destino fornecidos.
 *
 * @param origem Identificador do armazém de origem.
 * @param destino Identificador do armazém de destino.
 * @return Lista<int> Lista de IDs dos armazéns no caminho encontrado, ou vazia se não houver caminho.
 */
Lista<int> Transporte::calculaRota(int origem, int destino) {
    return this->_rede.BFS(origem, destino);
}

/**
 * @brief Calcula a rota entre dois armazéns considerando o peso das arestas.
 *
 * Este método utiliza o algoritmo de Dijkstra para encontrar
 * o caminho mais curto entre os armazéns com os IDs de origem e destino fornecidos.
 *
 * @param origem Identificador do armazém de origem.
 * @param destino Identificador do armazém de destino.
 * @return Lista<int> Lista de IDs dos armazéns no caminho encontrado, ou vazia se não houver caminho.
 */
Lista<int> Transporte::calculaRotaComPeso(int origem, int destino) {
    return this->_rede.Dykstra(origem, destino);
}

/**
 * @brief Obtém a rede de transporte.
 *
 * Este método retorna a instância de Grafo que representa a rede de armazéns e suas conexões.
 *
 * @return Grafo A rede de transporte.
 */
Grafo Transporte::getRede() const {
    return this->_rede;
}

/**
 * @brief Calcula a rota dinâmica entre dois armazéns considerando congestão.
 *
 * Este método implementa um algoritmo de Dijkstra otimizado que consulta
 * a congestão dos armazéns sob demanda, evitando a criação de matrizes custosas.
 * O peso dinâmico é calculado como peso_estático + peso_congestão_atual.
 *
 * @param origem Identificador do armazém de origem.
 * @param destino Identificador do armazém de destino.
 * @param armazens Vetor com os dados atuais dos armazéns para consulta de congestão.
 * @return Lista<int> Lista de IDs dos armazéns no caminho encontrado, ou vazia se não houver caminho.
 */
Lista<int> Transporte::calculaRotaDinamica(int origem, int destino, const Vetor<Armazem>& armazens) {
    int n = this->_rede.QuantidadeVertices();
    const int INF = 2147483647; // Valor máximo para representar infinito
    
    // Validação de entrada
    if (n == 0 || origem < 1 || origem > n || destino < 1 || destino > n) {
        std::cerr << "Índices de vértice inválidos em calculaRotaDinamica: origem=" << origem << ", destino=" << destino << std::endl;
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
    
    // Algoritmo de Dijkstra com consulta de congestão sob demanda
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
        Lista<Edge> vizinhos = this->_rede.GetVizinhosWeighted(u);
        auto aux = vizinhos._head ? vizinhos._head->GetNext() : nullptr;
        
        while (aux != nullptr) {
            Edge edge = aux->GetData();
            int vizinho = edge.destino;
            int peso_estatico = edge.peso;
            
            // Verifica se o vizinho está dentro dos limites
            if (vizinho >= 1 && vizinho <= n && !visitado[vizinho]) {
                // Consulta a congestão diretamente do armazém de origem
                int custo_congestion = 0;
                
                if (u <= armazens.getSize()) {
                    try {
                        // Cast para não-const necessário para acessar getDadosVizinho
                        Armazem& armazem_atual = const_cast<Armazem&>(armazens[u - 1]);
                        Vizinho* dados_vizinho = armazem_atual.getDadosVizinho(vizinho);
                        
                        if (dados_vizinho != nullptr) {
                            // Calcula peso de congestão: pacotes + (rearmazenamentos * multiplicador)
                            int pacotes_na_secao = dados_vizinho->pacotes.GetTam();
                            int rearmazenamentos = dados_vizinho->rearmazenamentos_recentes;
                            
                            custo_congestion = pacotes_na_secao + (rearmazenamentos * 3);
                            
                            // Inclui cooldown normalizado no cálculo de custo
                            int cooldown_atual = dados_vizinho->cooldown;
                            int cooldown_normalizado = cooldown_atual / 20;
                            custo_congestion += cooldown_normalizado;
                            
                        }
                    } catch (const std::exception&) {
                        custo_congestion = 0; // Em caso de erro, usa congestão zero
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

/**
 * @brief Decai a congestão de todos os armazéns da rede.
 *
 * Este método percorre todos os armazéns e decai a congestão de suas seções,
 * simulando a redução natural da congestão ao longo do tempo.
 *
 * @param armazens Vetor com os armazéns da rede (modificado in-place).
 */
void Transporte::decairCongestionTodosArmazens(Vetor<Armazem>& armazens) {
    int numArmazens = armazens.getSize();
    
    for (int i = 0; i < numArmazens; i++) {
        Armazem& armazem = armazens[i];
        Lista<int> vizinhos = armazem.getVizinhos();
        
        // Decai a congestão para cada vizinho deste armazém
        auto aux = vizinhos._head ? vizinhos._head->GetNext() : nullptr;
        while (aux != nullptr) {
            int idVizinho = aux->GetData();
            try {
                armazem.decayCongestion(idVizinho);
            } catch (const std::exception&) {
                // Se houver erro, simplesmente ignora e continua
            }
            aux = aux->GetNext();
        }
    }
}

#endif