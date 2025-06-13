#ifndef METRICAS_HPP
#define METRICAS_HPP

#include "lista.hpp"

class Metricas {
    private:
        Lista<double> _tempos;
        Lista<double> _distancias;
        double _tempo_total;
        double _distancia_total;
    public:
        Metricas();
        void adicionaTempo(double tempo);
        void adicionaDistancia(double distancia);
        double getTempoTotal() const;
        double getDistanciaTotal() const;
        Lista<double> getTempos() const;
        Lista<double> getDistancias() const;
};

// Implementation

inline Metricas::Metricas() : _tempo_total(0), _distancia_total(0) {}

inline void Metricas::adicionaTempo(double tempo) {
    this->_tempos.InsereFim(tempo);
    this->_tempo_total += tempo;
}

inline void Metricas::adicionaDistancia(double distancia) {
    this->_distancias.InsereFim(distancia);
    this->_distancia_total += distancia;
}

inline double Metricas::getTempoTotal() const {
    return this->_tempo_total;
}

inline double Metricas::getDistanciaTotal() const {
    return this->_distancia_total;
}

inline Lista<double> Metricas::getTempos() const {
    return this->_tempos;
}

inline Lista<double> Metricas::getDistancias() const {
    return this->_distancias;
}

#endif