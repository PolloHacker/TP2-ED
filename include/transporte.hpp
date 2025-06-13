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
        Lista<int> calculaRotaComPeso(int origem, int destino);
        Grafo getRede() const;
};

// Implementation

inline Transporte::Transporte() : _rede(Grafo()) {}

inline Armazem Transporte::adicionaArmazem(int id) {
    this->_rede.InsereVertice();
    return Armazem(id);
}

inline void Transporte::conectaArmazens(int id1, int id2) {
    this->_rede.InsereAresta(id1, id2);
}

inline void Transporte::imprimeRede() {
    for (int i = 1; i <= this->_rede.QuantidadeVertices(); ++i) {
        std::cout << "Armazém " << i << ": ";
        this->_rede.ImprimeVizinhos(i);
    }
}

inline Lista<int> Transporte::calculaRota(int origem, int destino) {
    return this->_rede.BFS(origem, destino);
}

inline Lista<int> Transporte::calculaRotaComPeso(int origem, int destino) {
    return this->_rede.BFS(origem, destino); // TODO: change back to Dykstra
}

inline Grafo Transporte::getRede() const {
    return this->_rede;
}

#endif