#ifndef HEAP_HPP
#define HEAP_HPP

#include "evento.hpp"
#include "vetor.hpp"

/*
* Chave da fila de prioridades:
* Dígitos: 19
*
*   1. Tempo: 6 dígitos
*   2. ID do pacote: 6 dígitos
*   3. ID do armazém de origem: 3 dígitos
*   4. ID do armazém de destino: 3 dígitos
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
        Evento Remover();

        bool Vazio();

};

#endif