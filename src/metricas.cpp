#include "metricas.hpp"

Metricas::Metricas() : _tempo_total(0), _distancia_total(0) {}

void Metricas::adicionaTempo(double tempo) {
    this->_tempos.InsereFim(tempo);
    this->_tempo_total += tempo;
}

void Metricas::adicionaDistancia(double distancia) {
    this->_distancias.InsereFim(distancia);
    this->_distancia_total += distancia;
}

double Metricas::getTempoTotal() const {
    return this->_tempo_total;
}

double Metricas::getDistanciaTotal() const {
    return this->_distancia_total;
}

Lista<double> Metricas::getTempos() const {
    return this->_tempos;
}

Lista<double> Metricas::getDistancias() const {
    return this->_distancias;
}