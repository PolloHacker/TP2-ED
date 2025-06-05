#include "escalonador.hpp"

Escalonador::Escalonador() 
    : _inicio(std::chrono::system_clock::now()), 
      _quantidadeEventos(0), 
      _tempoAtual(0) {}


void Escalonador::Inicializa() {
    this->_inicio = std::chrono::system_clock::now();
    this->_quantidadeEventos = 0;
    this->_tempoAtual = 0;
}

void Escalonador::InsereEvento() {
    // Logic to insert an event into the heap
    this->_quantidadeEventos++;
}

Evento Escalonador::RetiraProximoEvento() {
    if (this->_eventos.Vazio()) {
        throw std::runtime_error("No events to process.");
    }
    // Logic to remove the next event from the heap
    this->_quantidadeEventos--;
}

bool Escalonador::Vazio() {
    return this->_eventos.Vazio();
}

void Escalonador::Finaliza() {
    this->_fim = std::chrono::system_clock::now();
    this->_duracao = _fim - _inicio;
    // Logic to finalize the scheduler, e.g., logging the duration
    std::cout << "Scheduler finished. Duration: " << 
    this->_duracao.count() << " seconds." << std::endl;
}