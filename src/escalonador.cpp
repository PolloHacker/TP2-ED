#include "escalonador.hpp"

Escalonador::Escalonador() : _quantidadeEventos(0) { }

void Escalonador::Inicializa() {}

void Escalonador::InsereEvento(Evento& evento) {
    this->_eventos.Inserir(evento);
    this->_quantidadeEventos++;
}

Evento Escalonador::RetiraProximoEvento() {
    if (this->_eventos.Vazio()) {
        throw std::runtime_error("Não há eventos para retirar.");
    }
    // Logic to remove the next event from the heap
    Evento evento = this->_eventos.Remover();
    this->_quantidadeEventos--;

    return evento;
}

bool Escalonador::Vazio() {
    return this->_eventos.Vazio();
}

void Escalonador::Finaliza() {
    // TODO: Calcular métricas
}