#include "escalonador.hpp"

Escalonador::Escalonador() : _quantidadeEventos(0) { }

void Escalonador::Inicializa() {}

void Escalonador::InsereEvento(Evento& evento) {
    if (evento.getTempo() < this->_relogioDiscreto.getTempoAtual()) {
        throw std::invalid_argument("Tempo no evento não pode ser menor que o tempo atual do relógio discreto.");
    }
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
    this->_relogio.Finaliza();
    // TODO: Calcular métricas
}