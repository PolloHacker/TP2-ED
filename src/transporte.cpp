#include "transporte.hpp"

Transporte::Transporte() : _rede(Grafo()) {}

Armazem Transporte::adicionaArmazem(int id) {
    this->_rede.InsereVertice();
    
    return Armazem(id);
}

void Transporte::conectaArmazens(int id1, int id2) {
    this->_rede.InsereAresta(id1, id2);
}

void Transporte::imprimeRede() {
    for (int i = 1; i <= this->_rede.QuantidadeVertices(); ++i) {
        std::cout << "Armazém " << i << ": ";
        this->_rede.ImprimeVizinhos(i);
    }
}

Lista<int> Transporte::calculaRota(int origem, int destino) {
    return this->_rede.BFS(origem, destino);
}
Lista<int> Transporte::calculaRotaComPeso(int origem, int destino) {
    return this->_rede.BFS(origem, destino); // TODO: change back to Dykstra
}

Grafo Transporte::getRede() const {
    return this->_rede;
}