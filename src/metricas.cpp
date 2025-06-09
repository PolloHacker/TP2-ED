#include "metricas.hpp"

Metricas::Metricas() : _tempo_total(0), _distancia_total(0) {}

void Metricas::adicionaTempo(double tempo) {
    Node<double> aux = Node<double>(tempo);
    this->_tempos.InsereFim(aux);
    this->_tempo_total += tempo;
}

void Metricas::adicionaDistancia(double distancia) {
    Node<double> aux = Node<double>(distancia);
    this->_distancias.InsereFim(aux);
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