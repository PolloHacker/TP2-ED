#ifndef HEAP_HPP
#define HEAP_HPP

#include "evento.hpp"
#include "vetor.hpp"

/*
* Chave da fila de prioridades:
* Dígitos: 13
*
* 1. Tempo: 6 dígitos
*
* Se for pacote:
*   2. ID do pacote: 6 dígitos
* Se for transporte:
*   2. ID do armazém de origem: 3 dígitos
*   3. ID do armazém de destino: 3 dígitos
*
* (3/4). Tipo de evento: 1 dígito
*   1 - Postagem
*   2 - Transporte
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