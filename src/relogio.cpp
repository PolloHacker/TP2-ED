#include "relogio.hpp"

Relogio::Relogio() : _inicio(Clock::now()) {}

double Relogio::getTempoDecorrido() const {
    auto agora = Clock::now();
    std::chrono::duration<double> duracao = agora - this->_inicio;
    return duracao.count();
}

void Relogio::Finaliza() {
    this->_fim = Clock::now();
    this->_duracao = this->_fim - this->_inicio;
}

void Relogio::reset() {
    this->_inicio = Clock::now();
}