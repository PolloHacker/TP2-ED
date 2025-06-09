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
        Metricas() : _tempo_total(0), _distancia_total(0) {}

        void adicionaTempo(double tempo);

        void adicionaDistancia(double distancia);

        double getTempoTotal() const;

        double getDistanciaTotal() const;

        Lista<double> getTempos() const;

        Lista<double> getDistancias() const;

};

#endif