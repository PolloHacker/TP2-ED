#ifndef HEAP_HPP
#define HEAP_HPP

#include "evento.hpp"

class Heap{
    private:
        int tamanho;
        int capacidade;
        Evento* data;

        int GetAncestral(int posicao);
        int GetSucessorEsq(int posicao);
        int GetSucessorDir(int posicao);

        void Redimensionar();

        void HeapifyPorBaixo(int posicao);
        void HeapifyPorCima(int posicao);

    public:
        Heap();
        Heap(int maxsize);
        ~Heap();

        void Inserir(Evento evento);
        Evento Remover();

        bool Vazio();

};


#endif