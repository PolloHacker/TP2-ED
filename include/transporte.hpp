#ifndef TRANSPORTE_HPP
#define TRANSPORTE_HPP

#include "graph.hpp"
#include "armazem.hpp"

class Transporte {
    private:
        Grafo _rede;

    public:
        Transporte();

        void adicionaArmazem(int id);
        void conectaArmazens(int id1, int id2);
        
        void imprimeRede();

        Grafo getRede() const;
};

#endif