#ifndef ESCALONADOR_HPP
#define ESCALONADOR_HPP

#include <stdexcept>
#include <iostream>
#include "heap.hpp"

class Escalonador {
    private:
        Heap _eventos;
        int _quantidadeEventos;

    public:
        Escalonador();

        void Inicializa();

        void InsereEvento(Evento& evento);

        Evento RetiraProximoEvento();

        bool Vazio();

        void Finaliza();
};

#endif