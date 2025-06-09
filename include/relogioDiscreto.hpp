#ifndef RELOGIODISCRETO_HPP
#define RELOGIODISCRETO_HPP


#include <stdexcept>
#include <iostream>

class RelogioDiscreto {
    private:
        // O tempo é representado como um inteiro, onde cada unidade representa um passo de tempo discreto
        int _tempoAtual;

    public:
        RelogioDiscreto();

        int getTempoAtual() const;

        void avancarTempo(int incremento);

        void reset();
};

#endif