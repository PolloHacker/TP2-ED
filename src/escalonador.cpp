#include "escalonador.hpp"
#include "metricas.hpp"

Escalonador::Escalonador() : _quantidadeEventos(0) { }

void Escalonador::Inicializa() {}

void Escalonador::InsereEvento(Evento& evento, Metricas& metricas) {
    this->_eventos.Inserir(evento);
    this->_quantidadeEventos++;
    metricas.incHeapInsert();
}

Evento Escalonador::RetiraProximoEvento(Metricas& metricas) {
    if (this->_eventos.Vazio()) {
        throw std::runtime_error("Não há eventos para retirar.");
    }
    Evento evento = this->_eventos.Remover();
    this->_quantidadeEventos--;
    metricas.incHeapExtract();
    return evento;
}

bool Escalonador::Vazio() {
    return this->_eventos.Vazio();
}

void Escalonador::Finaliza() {
    // TODO: Calcular métricas
}