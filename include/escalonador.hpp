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

// Implementation

inline Escalonador::Escalonador() : _quantidadeEventos(0) { }

inline void Escalonador::Inicializa() {}

inline void Escalonador::InsereEvento(Evento& evento) {
    this->_eventos.Inserir(evento);
    this->_quantidadeEventos++;
}

inline Evento Escalonador::RetiraProximoEvento() {
    if (this->_eventos.Vazio()) {
        throw std::runtime_error("Não há eventos para retirar.");
    }
    Evento evento = this->_eventos.Remover();
    this->_quantidadeEventos--;
    return evento;
}

inline bool Escalonador::Vazio() {
    return this->_eventos.Vazio();
}

inline void Escalonador::Finaliza() {
    // TODO: Calcular métricas
}

#endif