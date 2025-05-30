#include "heap.hpp"

Heap::Heap(int maxsize): tamanho(0) {
    this->data = new int[maxsize];
}

void Heap::Inserir(int x) {
    this->data[tamanho] = x;
    this->HeapifyPorBaixo(tamanho++);
}

int Heap::Remover() {
    if (this->Vazio()) {
        return -1;
    }
    int aux = data[0];

    data[0] = data[tamanho - 1];
    tamanho--;
    HeapifyPorCima(0);
    return aux;
}

bool Heap::Vazio() {
    return this->tamanho == 0;
}

int Heap::GetAncestral(int posicao) {
    return (posicao - 1) / 2;
}

int Heap::GetSucessorDir(int posicao) {
    return (posicao * 2) + 2;
}

int Heap::GetSucessorEsq(int posicao) {
    return (posicao * 2) + 1;
}

void Heap::HeapifyPorBaixo(int posicao) {
    if (posicao == 0) return;
    
    int ant = this->GetAncestral(posicao);
    int aux;

    if (this->data[ant] > this->data[posicao]) {
        aux = this->data[ant];
        this->data[ant] = this->data[posicao];
        this->data[posicao] = aux;
    }
    
    this->HeapifyPorBaixo(ant);
}

void Heap::HeapifyPorCima(int posicao) {
    int suD = this->GetSucessorDir(posicao);
    int suE = this->GetSucessorEsq(posicao);

    if (suD > this->tamanho || suE > this->tamanho) return;

    int aux;
    int minSu = this->data[suE] < this->data[suD] ? suE : suD;

    if (this->data[minSu] < this->data[posicao]) {
        aux = this->data[minSu];
        this->data[minSu] = this->data[posicao];
        this->data[posicao] = aux;
    }

    this->HeapifyPorCima(minSu);
}

Heap::~Heap() {
    delete[] data;
}
