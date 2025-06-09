#include "transporte.hpp"

Transporte::Transporte() : _rede(Grafo()) {}

void Transporte::adicionaArmazem(int id) {
    this->_rede.InsereVertice();
    Armazem novoArmazem(id);
    this->_rede.InsereAresta(id, id); // Adiciona o armazém à lista de adjacência
}

void Transporte::conectaArmazens(int id1, int id2) {
    this->_rede.InsereAresta(id1, id2);
    this->_rede.InsereAresta(id2, id1); // Conexão bidirecional
}

void Transporte::imprimeRede() {
    for (int i = 1; i <= this->_rede.QuantidadeVertices(); ++i) {
        std::cout << "Armazém " << i << ": ";
        this->_rede.ImprimeVizinhos(i);
    }
}

Grafo Transporte::getRede() const {
    return this->_rede;
}