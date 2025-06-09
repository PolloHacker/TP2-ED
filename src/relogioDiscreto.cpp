#include "relogioDiscreto.hpp"

RelogioDiscreto::RelogioDiscreto() : _tempoAtual(0) {}

int RelogioDiscreto::getTempoAtual() const {
    return _tempoAtual;
}

void RelogioDiscreto::avancarTempo(int incremento) {
    if (incremento < 0) {
        throw std::invalid_argument("Incremento deve ser positivo.");
    }
    _tempoAtual += incremento;
}

void RelogioDiscreto::reset() {
    _tempoAtual = 0;
}
