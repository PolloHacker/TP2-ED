#ifndef ESCALONADOR_HPP
#define ESCALONADOR_HPP

#include <stdexcept>
#include <iostream>
#include <chrono>
#include "heap.hpp"
#include "relogio.hpp"
#include "relogioDiscreto.hpp"

class Escalonador {
    private:
        Relogio _relogio;
        RelogioDiscreto _relogioDiscreto;
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