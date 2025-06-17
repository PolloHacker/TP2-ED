#ifndef HEAP_HPP
#define HEAP_HPP

#include "utils.hpp"
#include "vetor.hpp"

/*
* Chave da fila de prioridades:
* Dígitos: 13
*
*   1. Tempo: 6 dígitos
*
*   2. ID do pacote: 6 dígitos 
*   ou 
*   2. ID do armazém de origem: 3 dígitos
*   3. ID do armazém de destino: 3 dígitos
*
*   5. Tipo de evento: 1 dígito
*/

class Heap{
    private:
        int _tamanho;
        Vetor<Evento> _data;

        int GetAncestral(int posicao);
        int GetSucessorEsq(int posicao);
        int GetSucessorDir(int posicao);

        void HeapifyPorBaixo(int posicao);
        void HeapifyPorCima(int posicao);

    public:
        Heap();
        Heap(int maxsize);

        void Inserir(Evento evento);
        Evento Remover();        bool Vazio();

};

Heap::Heap(): _tamanho(0), _data(10) {}

Heap::Heap(int maxsize): _tamanho(0), _data(maxsize) {}

void Heap::Inserir(Evento evento) {
    this->_data.insere(this->_tamanho, evento);
    this->HeapifyPorBaixo(this->_tamanho++);
}

Evento Heap::Remover() {
    if (this->Vazio()) {
        return Evento(-1, -1, -1, -1, TipoEvento::CHEGADA_PACOTE);
    }
    Evento aux = this->_data[0];

    this->_data[0] = _data[this->_tamanho - 1];
    this->_tamanho--;
    HeapifyPorCima(0);
    return aux;
}

bool Heap::Vazio() {
    return this->_tamanho == 0;
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

    if (this->_data[ant] > this->_data[posicao]) {
        Evento aux = this->_data[ant];
        this->_data[ant] = this->_data[posicao];
        this->_data[posicao] = aux;
    }
    
    this->HeapifyPorBaixo(ant);
}

void Heap::HeapifyPorCima(int posicao) {
    int suD = this->GetSucessorDir(posicao);
    int suE = this->GetSucessorEsq(posicao);

    if (suD > this->_tamanho || suE > this->_tamanho) return;

    int minSu = this->_data[suE] < this->_data[suD] ? suE : suD;

    if (this->_data[minSu] < this->_data[posicao]) {
        Evento aux = this->_data[minSu];
        this->_data[minSu] = this->_data[posicao];
        this->_data[posicao] = aux;
    }

    this->HeapifyPorCima(minSu);
}

#endif