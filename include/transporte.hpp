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
        void conectaArmazens(int id1, int id2);
        
        void imprimeRede();

        Lista<int> calculaRota(int origem, int destino);
        Lista<int> calculaRotaComPeso(int origem, int destino);        Grafo getRede() const;
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
 */
void Transporte::conectaArmazens(int id1, int id2) {
    this->_rede.InsereAresta(id1, id2);
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
    return this->_rede.BFS(origem, destino); // TODO: change back to Dykstra
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

#endif