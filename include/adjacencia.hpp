#ifndef ADJACENCIA_HPP
#define ADJACENCIA_HPP

#include <iostream>
#include "lista.hpp"

class ListaAdjacencia {
    private:
        Lista<Lista<int>> _lista;
        int _vertices;

    public:
        ListaAdjacencia();
        ~ListaAdjacencia();

        void InsereVertice();
        void InsereAresta(int v, int w);

        int GetVertices();
        int GetArestas();

        Lista<int> GetVizinhos(int v);    
};

// Implementation

inline ListaAdjacencia::ListaAdjacencia(): _lista(), _vertices(0) {}

inline int ListaAdjacencia::GetVertices() {
    return this->_vertices;
}

inline int ListaAdjacencia::GetArestas() {
    int count = 0;
    Node<int>* current;
    for (int i = 1; i <= this->_vertices; i++) {
        Lista<int> vizinhos = this->_lista.GetElemPos(i)->GetData();
        current = vizinhos._head->GetNext();
        while (current != nullptr) {
            if (current->GetData() + 1 > i) {
                count++;
            }
            current = current->GetNext();
        }
    }
    return count;
}

inline void ListaAdjacencia::InsereVertice() {
    Lista<int> novaLista;
    this->_lista.InsereFim(novaLista);
    this->_vertices++;
}

inline void ListaAdjacencia::InsereAresta(int from, int to) {
    Lista<int> vizinhos = this->_lista.GetElemPos(from)->GetData();
    vizinhos.InsereFim(to);
    this->_lista.GetElemPos(from)->SetData(vizinhos);
}

inline Lista<int> ListaAdjacencia::GetVizinhos(int v) {
    if (v < 1 || v > this->_vertices) {
        throw std::out_of_range("Índice de vértice inválido.");
    }
    return this->_lista.GetElemPos(v)->GetData();
}

inline ListaAdjacencia::~ListaAdjacencia() {
    this->_lista.~Lista();
}

#endif