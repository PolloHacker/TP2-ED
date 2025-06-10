#ifndef TRANSPORTE_HPP
#define TRANSPORTE_HPP

#include "graph.hpp"
#include "armazem.hpp"

class Transporte {
    private:
        Grafo _rede;

    public:
        Transporte();

        Armazem adicionaArmazem(int id);
        void conectaArmazens(int id1, int id2);
        
        void imprimeRede();

        Lista<int> calculaRota(int origem, int destino);
        Lista<int> calculaRotaComPeso(int origem, int destino);

        Grafo getRede() const;
};

#endif