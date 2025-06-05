#include "heap.hpp"

Heap::Heap(): tamanho(0), capacidade(50) {
    this->data = new Evento[50]; // Inicializa com tamanho padrão de 50
}

Heap::Heap(int maxsize): tamanho(0) {
    this->data = new Evento[maxsize];
}

void Heap::Redimensionar() {
    int novaCapacidade = capacidade * 2;
    Evento* novoData = new Evento[novaCapacidade];
    
    for (int i = 0; i < tamanho; i++) {
        novoData[i] = data[i];
    }
    
    delete[] data;
    data = novoData;
    capacidade = novaCapacidade;
}

void Heap::Inserir(Evento evento) {
    if (tamanho >= capacidade) {
        this->Redimensionar();
    }
    
    this->data[tamanho] = evento;
    this->HeapifyPorBaixo(tamanho++);
}

Evento Heap::Remover() {
    if (this->Vazio()) {
        return Evento(-1, -1, TipoEvento::POR_POSTAR);
    }
    Evento aux = data[0];

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

    if (this->data[ant] > this->data[posicao]) {
        Evento aux = this->data[ant];
        this->data[ant] = this->data[posicao];
        this->data[posicao] = aux;
    }
    
    this->HeapifyPorBaixo(ant);
}

void Heap::HeapifyPorCima(int posicao) {
    int suD = this->GetSucessorDir(posicao);
    int suE = this->GetSucessorEsq(posicao);

    if (suD > this->tamanho || suE > this->tamanho) return;

    int minSu = this->data[suE] < this->data[suD] ? suE : suD;

    if (this->data[minSu] < this->data[posicao]) {
        Evento aux = this->data[minSu];
        this->data[minSu] = this->data[posicao];
        this->data[posicao] = aux;
    }

    this->HeapifyPorCima(minSu);
}

Heap::~Heap() {
    delete[] data;
}
