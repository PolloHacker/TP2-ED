#include "adjacencia.hpp"

ListaAdjacencia::ListaAdjacencia(): _lista(), _vertices(0) {}

int ListaAdjacencia::GetVertices() {
    return this->_vertices;
}

int ListaAdjacencia::GetArestas() {
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

void ListaAdjacencia::InsereVertice() {
    Lista<int> novaLista;
    std::cout << "Tamanho da lista de adj " << this->_lista.GetTam() << std::endl;
    this->_lista.InsereFim(novaLista);
    this->_vertices++;
}

void ListaAdjacencia::InsereAresta(int from, int to) {
    Lista<int> vizinhos = this->_lista.GetElemPos(from)->GetData();
    vizinhos.InsereFim(to);
    this->_lista.GetElemPos(from)->SetData(vizinhos);
}

Lista<int> ListaAdjacencia::GetVizinhos(int v) {
    if (v < 1 || v > this->_vertices) {
        throw std::out_of_range("Índice de vértice inválido.");
    }
    return this->_lista.GetElemPos(v)->GetData();
}


ListaAdjacencia::~ListaAdjacencia() {
    this->_lista.~Lista();
}