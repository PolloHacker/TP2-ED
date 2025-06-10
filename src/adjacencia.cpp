#include "adjacencia.hpp"

ListaAdjacencia::ListaAdjacencia(): _vertices(0) {
    this->_lista = Lista<Lista<int>>();
}

int ListaAdjacencia::GetVertices() {
    return this->_vertices;
}

int ListaAdjacencia::GetArestas() {
    int count = 0;
    Node<int>* current;
    for (int i = 1; i <= this->_vertices; i++) {
        Lista<int> vizinhos = this->_lista.GetElemPos(i)->GetData();
        current = vizinhos._head;
        while (current != nullptr) {
            if (current->GetData() + 1 > i) {
                count++;
            }
            current = current->GetNext();
        }
    }
    delete current;

    return count;
}

void ListaAdjacencia::InsereVertice() {
    Node<Lista<int>> novaLista = Node<Lista<int>>(Lista<int>());
    this->_lista.InsereFim(novaLista);
    this->_vertices++;
}

void ListaAdjacencia::InsereAresta(int from, int to) {
    Node<int> node(to);
    Lista<int> vizinhos = this->_lista.GetElemPos(from)->GetData();
    vizinhos.InsereFim(node);
    this->_lista.GetElemPos(from)->SetData(vizinhos);
}

Lista<int> ListaAdjacencia::GetVizinhos(int v) {
    return this->_lista.GetElemPos(v)->GetData();
}


ListaAdjacencia::~ListaAdjacencia() {
    this->_lista.~Lista();
}