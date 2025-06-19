#include "metricas.hpp"

Metricas::Metricas() : _tempo_total(0), _distancia_total(0) {}

void Metricas::adicionaTempo(int tempo) {
    this->_tempos.InsereFim(tempo);
    this->_tempo_total += tempo;
}

void Metricas::adicionaDistancia(int distancia) {
    this->_distancias.InsereFim(distancia);
    this->_distancia_total += distancia;
}

int Metricas::getTempoTotal() const {
    return this->_tempo_total;
}

int Metricas::getDistanciaTotal() const {
    return this->_distancia_total;
}

Lista<int> Metricas::getTempos() const {
    return this->_tempos;
}

Lista<int> Metricas::getDistancias() const {
    return this->_distancias;
}