#ifndef EVENTO_HPP
#define EVENTO_HPP

#include <stdexcept>
#include <string>
#include "vetor.hpp"

enum class TipoEvento {
    POSTAGEM = 1,
    TRANSPORTE = 2
};

class Evento {
    private:
        std::string _chave;
        int _tempo;


    public:
        Evento();
        Evento(std::string data);
        Evento(int tempo, int idPacote, TipoEvento tipoEvento);
        Evento(int tempo, int idArmazemOrigem, int idArmazemDestino, TipoEvento tipoEvento);

        std::string getData() const;
        int getTempo() const;
        int getIdPacote() const;
        Vetor<int> getArmazens() const;
        TipoEvento getTipoEvento() const;

        bool operator<(const Evento& other) const;
        bool operator>(const Evento& other) const;
        bool operator==(const Evento& other) const;
        bool operator!=(const Evento& other) const;
        bool operator<=(const Evento& other) const;
        bool operator>=(const Evento& other) const;
};

#endif