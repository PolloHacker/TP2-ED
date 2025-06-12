#pragma once

#include "pilha.hpp"

template <typename T>
Pilha<T>::Pilha() : _topo(nullptr) {}

template <typename T>
Pilha<T>::Pilha(const Pilha<T>& other) : _topo(nullptr) {
    if (other._topo == nullptr) {
        return; // Pilha vazia
    }
    Node<T>* current = other._topo;
    Node<T>* prev = nullptr;
    while (current != nullptr) {
        Node<T>* novo = new Node<T>(current->GetData());
        if (prev == nullptr) {
            this->_topo = novo; // Primeiro elemento
        } else {
            prev->SetNext(novo);
        }
        prev = novo;
        current = current->GetNext();
    }
}

template <typename T>
Pilha<T>& Pilha<T>::operator=(const Pilha<T>& other) {
    if (this != &other) {
        // Limpa a pilha atual
        while (!this->Vazia()) {
            this->Desempilha();
        }
        // Copia os elementos da outra pilha
        Node<T>* current = other._topo;
        Node<T>* prev = nullptr;
        while (current != nullptr) {
            Node<T>* novo = new Node<T>(current->GetData());
            if (prev == nullptr) {
                this->_topo = novo; // Primeiro elemento
            } else {
                prev->SetNext(novo);
            }
            prev = novo;
            current = current->GetNext();
        }
    }
    return *this;
}

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
int Pilha<T>::GetTam() const {
    int count = 0;
    Node<T>* current = this->_topo;
    while (current != nullptr) {
        count++;
        current = current->GetNext();
    }
    return count;
}

template <typename T>
Pilha<T>::~Pilha() {
    while (!this->Vazia()) {
        this->Desempilha();
    }
}