#pragma once

#include "node.hpp"

template <typename T>
Node<T>::Node() : _data(T()), _next(nullptr) {}

template <typename T>
Node<T>::Node(T val) : _data(val), _next(nullptr) {}

template <typename T>
Node<T>::Node(const Node<T>& other) : _data(other._data), _next(nullptr) {}

template <typename T>
Node<T>& Node<T>::operator=(const Node<T>& other) {
    if (this != &other) {
        this->_data = other._data;
        this->_next = nullptr;
    }
    return *this;
}

template <typename T>
T& Node<T>::GetDataRef() {
    return this->_data;
}

template <typename T>
T& Node<T>::GetData() {
    return this->_data;
}

template <typename T>
Node<T>* Node<T>::GetNext() {
    return this->_next;
}

template <typename T>
void Node<T>::SetData(T val) {
    this->_data = val;
}

template <typename T>
void Node<T>::SetNext(Node<T>* obj) {
    this->_next = obj;
}
