#pragma once

#include "lista.hpp"

template <typename T>
Lista<T>::Lista() : _tam(0) {
    Node<T>* aux = new Node<T>();
    this->_head = aux;
    this->_tail = aux;
}

template <typename T>
Node<T>* Lista<T>::Posiciona(int pos) {
    int i;
    Node<T>* aux;

    if (this->_tam < pos || pos <= 0) {
        throw std::invalid_argument("Posicao invalida");
    }

    aux = this->_head;

    for (i = 1; i <= pos ; i++) {
        aux = aux->GetNext();
    }

    return aux;
}

template <typename T>
void Lista<T>::InsereInicio(Node<T>& obj) {
    Node<T>* entry = new Node<T>(std::forward<Node<T>>(obj));
    entry->SetNext(this->_head->GetNext());
    this->_head->SetNext(entry);
    this->_tam++;

    if (this->_tail == nullptr) {
        this->_tail = entry;
    }
}

template <typename T>
void Lista<T>::InserePos(int pos, Node<T>& obj) {
    Node<T>* before = this->Posiciona(pos - 1);
    Node<T>* entry = new Node<T>(std::forward<Node<T>>(obj));

    entry->SetNext(before->GetNext());
    before->SetNext(entry);
    this->_tam++;
    
    if (entry->GetNext() == nullptr) {
        this->_tail = entry;
    }
}

template <typename T>
void Lista<T>::InsereFim(Node<T>& obj) {
    Node<T>* entry = new Node<T>(std::forward<Node<T>>(obj));
    this->_tail->SetNext(entry);
    this->_tail = entry;
    this->_tam++;
}

template <typename T>
Node<T> Lista<T>::RemovePos(int pos) {
    if (this->_tam == 0) {
        throw std::invalid_argument("Lista vazia");
    }

    Node<T>* before = this->Posiciona(pos - 1);
    Node<T>* to_del = before->GetNext();

    before->SetNext(to_del->GetNext());
    this->_tam--;

    if (before->GetNext() == nullptr) {
        this->_tail = before;
    }

    return to_del->GetData();
}


template <typename T>
Node<T>* Lista<T>::GetElemPos(int pos) {
    Node<T>* aux;

    aux = this->Posiciona(pos);
    return aux;
}


template <typename T>
int Lista<T>::GetTam() {
    return this->_tam;
}


template <typename T>
Lista<T>::~Lista() {
    delete[] this->_lista;
    delete this->_head;
    delete this->_tail;
}