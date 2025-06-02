#include <stdexcept>
#include "fila.hpp"

Fila::Fila() : _inicio(nullptr), _fim(nullptr) {}

void Fila::Enfileira(int valor) {
    Node<int>* novo = new Node<int>(valor);
    if (Vazia()) {
        this->_inicio = novo;
        this->_fim = novo;
    } else {
        this->_fim->SetNext(novo);
        this->_fim = novo;
    }
}

int Fila::Desenfileira() {
    if (Vazia()) {
        throw std::runtime_error("Fila vazia");
    }
    Node<int>* temp = this->_inicio;
    int valor = temp->GetData();
    this->_inicio = this->_inicio->GetNext();
    if (this->_inicio == nullptr) {
        this->_fim = nullptr; // Se a fila ficar vazia, atualiza o fim
    }
    delete temp;
    return valor;
}

int Fila::Frente() const {
    if (Vazia()) {
        throw std::runtime_error("Fila vazia");
    }
    return this->_inicio->GetData();
}

bool Fila::Vazia() const {
    return this->_inicio == nullptr;
}

Fila::~Fila() {
    while (!Vazia()) {
        Desenfileira();
    }
}