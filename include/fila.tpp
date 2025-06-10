#pragma once 

#include "fila.hpp"

template <typename T>
Fila<T>::Fila() : _inicio(nullptr), _fim(nullptr) {}

template <typename T>
void Fila<T>::Enfileira(T valor) {
    Node<T>* novo = new Node<T>(valor);
    if (Vazia()) {
        this->_inicio = novo;
        this->_fim = novo;
    } else {
        this->_fim->SetNext(novo);
        this->_fim = novo;
    }
}

template <typename T>
T Fila<T>::Desenfileira() {
    if (Vazia()) {
        throw std::runtime_error("Fila vazia");
    }
    Node<T>* temp = this->_inicio;
    T valor = temp->GetData();
    this->_inicio = this->_inicio->GetNext();
    if (this->_inicio == nullptr) {
        this->_fim = nullptr; // Se a fila ficar vazia, atualiza o fim
    }
    delete temp;
    return valor;
}

template <typename T>
T Fila<T>::Frente() const {
    if (this->Vazia()) {
        throw std::runtime_error("Fila vazia");
    }
    return this->_inicio->GetData();
}

template <typename T>
bool Fila<T>::Vazia() const {
    return this->_inicio == nullptr;
}

template <typename T>
Fila<T>::~Fila() {
    while (!this->Vazia()) {
        this->Desenfileira();
    }
}