#include "transporte.hpp"

template <typename T>
Transporte<T>::Transporte() : _rede(Grafo<Armazem>()) {}

template <typename T>
void Transporte<T>::adicionaArmazem(int id) {
    this->_rede.InsereVertice();
    Armazem novoArmazem(id);
    this->_rede.InsereAresta(id, id); // Adiciona o armazém à lista de adjacência
}

template <typename T>
void Transporte<T>::conectaArmazens(int id1, int id2) {
    this->_rede.InsereAresta(id1, id2);
    this->_rede.InsereAresta(id2, id1); // Conexão bidirecional
}

template <typename T>
void Transporte<T>::imprimeRede() const {
    for (int i = 1; i <= this->_rede.QuantidadeVertices(); ++i) {
        std::cout << "Armazém " << i << ": ";
        this->_rede.ImprimeVizinhos(i);
    }
}

template <typename T>
Grafo<Armazem> Transporte<T>::getRede() const {
    return this->_rede;
}