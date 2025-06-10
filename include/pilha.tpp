#pragma once

#include "pilha.hpp"

template <typename T>
Pilha<T>::Pilha() : _topo(nullptr) {}

template <typename T>
void Pilha<T>::Empilha(T valor) {
    Node<T>* novo = new Node<T>(valor);
    novo->SetNext(this->_topo);
    this->_topo = novo;
}

template <typename T>
T Pilha<T>::Desempilha() {
    if (Vazia()) {
        throw std::runtime_error("Pilha vazia");
    }
    Node<T>* temp = this->_topo;
    T valor = temp->GetData();
    this->_topo = this->_topo->GetNext();
    delete temp;
    return valor;
}

template <typename T>
T Pilha<T>::Topo() const {
    if (Vazia()) {
        throw std::runtime_error("Pilha vazia");
    }
    return this->_topo->GetData();
}

template <typename T>
bool Pilha<T>::Vazia() const {
    return this->_topo == nullptr;
}

template <typename T>
Pilha<T>::~Pilha() {
    while (!this->Vazia()) {
        this->Desempilha();
    }
}