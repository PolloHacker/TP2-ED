#include <stdexcept>
#include "pilha.hpp"

Pilha::Pilha() : _topo(nullptr) {}

void Pilha::Empilha(int valor) {
    Node<int>* novo = new Node<int>(valor);
    novo->SetNext(this->_topo);
    this->_topo = novo;
}

int Pilha::Desempilha() {
    if (Vazia()) {
        throw std::runtime_error("Pilha vazia");
    }
    Node<int>* temp = this->_topo;
    int valor = temp->GetData();
    this->_topo = this->_topo->GetNext();
    delete temp;
    return valor;
}

int Pilha::Topo() const {
    if (Vazia()) {
        throw std::runtime_error("Pilha vazia");
    }
    return this->_topo->GetData();
}

bool Pilha::Vazia() const {
    return this->_topo == nullptr;
}


Pilha::~Pilha() {
    while (!Vazia()) {
        Desempilha();
    }
}

