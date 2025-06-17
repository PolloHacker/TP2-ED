#ifndef LISTA_HPP
#define LISTA_HPP

#include <iostream>
#include <exception>
#include <string>
#include "node.hpp"

template <typename T>
class Lista {
    private:
        int _tam;
        
    public:
        Lista();
        Lista(const Lista<T>& other);
        Lista<T>& operator=(const Lista<T>& other);
        ~Lista();

        Node<T>* Posiciona(int pos);
        void InsereInicio(T& obj);
        void InserePos(int pos, T& obj);
        void InsereFim(T& obj);
        
        T RemovePos(int pos);
        Node<T>* GetElemPos(int pos);

        int GetTam();
        Node<T>* _head;
        Node<T>* _tail;
        

};

template <typename T>
Lista<T>::Lista() : _tam(0) {
    Node<T>* aux = new Node<T>();
    this->_head = aux;
    this->_tail = aux;
}

template <typename T>
Lista<T>::Lista(const Lista<T>& other) : _tam(other._tam) {
    this->_head = new Node<T>();
    Node<T>* current = this->_head;

    Node<T>* otherCurrent = other._head->GetNext();
    while (otherCurrent != nullptr) {
        Node<T>* newNode = new Node<T>(otherCurrent->GetData());
        current->SetNext(newNode);
        current = newNode;
        otherCurrent = otherCurrent->GetNext();
    }
    this->_tail = current;
}

template <typename T>
Lista<T>& Lista<T>::operator=(const Lista<T>& other) {
    if (this != &other) {
        // Limpa a lista atual
        Node<T>* current = this->_head;
        while (current != nullptr) {
            Node<T>* next = current->GetNext();
            delete current;
            current = next;
        }
        
        // Copia os dados da outra lista
        this->_tam = other._tam;
        this->_head = new Node<T>();
        Node<T>* newCurrent = this->_head;

        Node<T>* otherCurrent = other._head->GetNext();
        while (otherCurrent != nullptr) {
            Node<T>* newNode = new Node<T>(otherCurrent->GetData());
            newCurrent->SetNext(newNode);
            newCurrent = newNode;
            otherCurrent = otherCurrent->GetNext();
        }
        this->_tail = newCurrent;
    }
    return *this;
}

template <typename T>
Node<T>* Lista<T>::Posiciona(int pos) {
    int i;
    Node<T>* aux;

    if (this->_tam < pos || pos < 0) {
        throw std::invalid_argument("Posicao invalida");
    }

    aux = this->_head;

    for (i = 1; i <= pos ; i++) {
        aux = aux->GetNext();
    }

    return aux;
}

template <typename T>
void Lista<T>::InsereInicio(T& obj) {
    Node<T>* entry = new Node<T>(obj);
    entry->SetNext(this->_head->GetNext());
    this->_head->SetNext(entry);
    this->_tam++;

    if (this->_tail == nullptr) {
        this->_tail = entry;
    }
}

template <typename T>
void Lista<T>::InserePos(int pos, T& obj) {
    Node<T>* before = this->Posiciona(pos - 1);
    Node<T>* entry = new Node<T>(obj);

    entry->SetNext(before->GetNext());
    before->SetNext(entry);
    this->_tam++;
    
    if (entry->GetNext() == nullptr) {
        this->_tail = entry;
    }
}

template <typename T>
void Lista<T>::InsereFim(T& obj) {
    Node<T>* entry = new Node<T>(obj);
    this->_tail->SetNext(entry);
    this->_tail = entry;
    this->_tam++;
}

template <typename T>
T Lista<T>::RemovePos(int pos) {
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

    T data = to_del->GetData();
    delete to_del;

    return data;
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
    Node<T>* curr = this->_head;

    while (curr != nullptr) {
        Node<T>* next = curr->GetNext();
        delete curr;
        curr = next;
    }
    this->_head = nullptr;
    this->_tail = nullptr;
}

#endif