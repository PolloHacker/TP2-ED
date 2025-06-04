#include "escalonador.hpp"

Escalonador::Escalonador() 
    : _inicio(std::chrono::system_clock::now()), 
      _quantidadeEventos(0), 
      _tempoAtual(0) {}

Escalonador::~Escalonador() {
    // Destructor logic if needed
}
void Escalonador::Inicializa() {
    _inicio = std::chrono::system_clock::now();
    _quantidadeEventos = 0;
    _tempoAtual = 0;
}

void Escalonador::InsereEvento() {
    // Logic to insert an event into the heap
    _quantidadeEventos++;
}

void Escalonador::RetiraProximoEvento() {
    if (_eventos.Vazia()) {
        throw std::runtime_error("No events to process.");
    }
    // Logic to remove the next event from the heap
    _quantidadeEventos--;
}

void Escalonador::Finaliza() {
    _fim = std::chrono::system_clock::now();
    _duracao = _fim - _inicio;
    // Logic to finalize the scheduler, e.g., logging the duration
    std::cout << "Scheduler finished. Duration: " << _duracao.count() << " seconds." << std::endl;
}