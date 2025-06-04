#ifndef ESCALONADOR_HPP
#define ESCALONADOR_HPP

#include <chrono>
#include "heap.hpp"

class Escalonador {
    private:
        Heap _eventos;
        std::chrono::time_point<std::chrono::system_clock> _inicio;
        std::chrono::time_point<std::chrono::system_clock> _fim;
        std::chrono::duration<double> _duracao;
        int _quantidadeEventos;
        int _tempoAtual;

    public:
        Escalonador();
        ~Escalonador();

        void Inicializa();

        void InsereEvento();

        void RetiraProximoEvento();

        void Finaliza();
};

#endif