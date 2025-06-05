#ifndef EVENTO_HPP
#define EVENTO_HPP

#include <string>

enum class TipoEvento {
    POR_POSTAR,
    POSTADO,
    POR_ARMAZENAR,
    ARMAZENADO,
    EM_ROTA,
    ENTREGUE
};

class Evento {
    private:
        int _id;
        double _tempo;
        TipoEvento _tipo;


    public:
        Evento();
        Evento(int tempo, int id, const TipoEvento& tipo);

        double getTempo() const;
        int getId() const;
        TipoEvento getTipo() const;

        void setTempo(double tempo);
        void setId(int id);
        void setTipo(const TipoEvento& tipo);

        bool operator<(const Evento& other) const;
        bool operator>(const Evento& other) const;
        bool operator==(const Evento& other) const;
        bool operator!=(const Evento& other) const;
        bool operator<=(const Evento& other) const;
        bool operator>=(const Evento& other) const;
};

#endif