#ifndef ESCALONADOR_HPP
#define ESCALONADOR_HPP

#include <chrono>

class Escalonador {
    public:
        Escalonador();
        ~Escalonador();

        void Inicializa();

        void InsereEvento();

        void RetiraProximoEvento();

        void Finaliza();
};

#endif