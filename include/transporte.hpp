#ifndef TRANSPORTE_HPP
#define TRANSPORTE_HPP

#include "graph.hpp"
#include "armazem.hpp"

template <typename T>
class Transporte {
    private:
        Grafo<Armazem> _rede;

    public:
        Transporte();

        void adicionaArmazem(int id);
        void conectaArmazens(int id1, int id2);
        
        void imprimeRede() const;

        Grafo<Armazem> getRede() const;
};

#endif