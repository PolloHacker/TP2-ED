#ifndef PILHA_HPP
#define PILHA_HPP

#include <stdexcept>
#include "node.hpp"

template <typename T>
class Pilha {
    private:
        Node<T>* _topo;
        
    public:
        Pilha();
        Pilha(const Pilha<T>& other);
        Pilha<T>& operator=(const Pilha<T>& other);
        ~Pilha();

        void Empilha(T valor);
        T Desempilha();
        T Topo() const;
        bool Vazia() const;

        int GetTam() const;

};

// Implementation

template <typename T>
Pilha<T>::Pilha() : _topo(nullptr) {}

template <typename T>
Pilha<T>::Pilha(const Pilha<T>& other) : _topo(nullptr) {
    if (other._topo == nullptr) {
        return;
    }
    Node<T>* current = other._topo;
    Node<T>* prev = nullptr;
    while (current != nullptr) {
        Node<T>* novo = new Node<T>(current->GetData());
        if (prev == nullptr) {
            this->_topo = novo;
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
        while (!this->Vazia()) {
            this->Desempilha();
        }
        Node<T>* current = other._topo;
        Node<T>* prev = nullptr;
        while (current != nullptr) {
            Node<T>* novo = new Node<T>(current->GetData());
            if (prev == nullptr) {
                this->_topo = novo;
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

#endif