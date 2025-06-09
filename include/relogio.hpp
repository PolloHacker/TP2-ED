#ifndef RELOGIO_HPP
#define RELOGIO_HPP

#include <chrono>

using Clock = std::chrono::steady_clock;
using TimePoint = Clock::time_point;

class Relogio {

    private:
        TimePoint _inicio;
        TimePoint _fim;
        std::chrono::duration<double> _duracao;

    public:
        Relogio();

        double getTempoDecorrido() const;

        void Finaliza();
        void reset();
};

#endif