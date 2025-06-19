#ifndef METRICAS_HPP
#define METRICAS_HPP

#include "lista.hpp"

class Metricas {
    private:
        Lista<int> _tempos;
        Lista<int> _distancias;
        int _tempo_total;
        int _distancia_total;
    public:
        Metricas();

        void adicionaTempo(int tempo);

        void adicionaDistancia(int distancia);

        int getTempoTotal() const;

        int getDistanciaTotal() const;

        Lista<int> getTempos() const;

        Lista<int> getDistancias() const;

};

#endif