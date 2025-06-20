#ifndef ESCALONADOR_HPP
#define ESCALONADOR_HPP

#include <stdexcept>
#include <iostream>
#include "heap.hpp"
#include "metricas.hpp"

class Escalonador {
    private:
        Heap _eventos;
        int _quantidadeEventos;

    public:
        Escalonador();

        void Inicializa();

        void InsereEvento(Evento& evento, Metricas& metricas);

        Evento RetiraProximoEvento(Metricas& metricas);

        bool Vazio();

        void Finaliza();
};

#endif