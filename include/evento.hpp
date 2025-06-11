#ifndef EVENTO_HPP
#define EVENTO_HPP

#include <stdexcept>
#include <string>
#include "vetor.hpp"

enum class TipoEvento {
    PACOTE = 1,
    TRANSPORTE,
    REMOCAO,
    REARMAZENAMENTO,
    ENTREGA 
};

enum class CustoEvento {
    CAPACIDADE_TRANSPORTE,
    LATENCIA_TRANSPORTE,
    INTERVALO_TRANSPORTE,
    CUSTO_REMOVER_PACOTE,
};

class Evento {
    private:
        std::string _chave;
        int _tempo;


    public:
        Evento();
        Evento(std::string data);
        Evento(int tempo, int idPacote, int idArmazemOrigem, int idArmazemDestino, TipoEvento tipoEvento);

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