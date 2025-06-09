#include "node.hpp"
#include "lista.hpp"
#include <type_traits>

template <typename T>
Node<T>::Node() : _data(T()), _next(nullptr) {}


template <>
Node<Lista<int>>::Node() : _data(Lista<int>()), _next(nullptr) {}

template <>
Node<int>::Node() : _data(-1), _next(nullptr) {}

template <typename T>
Node<T>::Node(T val) : _data(val), _next(nullptr) {}

template <typename T>
T Node<T>::GetData() {
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

template <typename T>
Node<T>::~Node() {
    delete this->_next;
}

template class Node<int>;
template class Node<Lista<int>>;