#ifndef EVENTO_HPP
#define EVENTO_HPP

#include <stdexcept>
#include <iomanip>
#include <sstream>
#include <string>
#include "vetor.hpp"

enum class TipoEvento {
    CHEGADA_PACOTE = 1,
    TRANSPORTE = 2
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

// Implementation

inline Evento::Evento() : _chave(""), _tempo(-1) {}

inline Evento::Evento(std::string chave) : _chave(chave) {
    if (chave.length() != 13) {
        throw std::invalid_argument("Chave deve ter pelo menos 13 caracteres.");
    }
    this->_tempo = std::stoi(chave.substr(0, 6));
}

inline Evento::Evento(int tempo, int idPacote, int idArmazemOrigem, int idArmazemDestino, TipoEvento tipoEvento) : _tempo(tempo) {
    int tipo = 0;
    if (tempo < 0) {
        throw std::invalid_argument("Tempo não pode ser negativo.");
    }
    switch (tipoEvento) {
        case TipoEvento::CHEGADA_PACOTE: tipo = 1; break;
        case TipoEvento::TRANSPORTE: tipo = 2; break;
        default:
            throw std::invalid_argument("Tipo de evento inválido.");
    }
    std::ostringstream oss;
    oss << std::setfill('0');
    oss << std::setw(6) << tempo;
    if (idPacote >= 0) {
        oss << std::setw(6) << idPacote;
        oss << tipo;
    } else if (idArmazemOrigem >= 0 && idArmazemDestino >= 0) {
        oss << std::setw(3) << idArmazemOrigem;
        oss << std::setw(3) << idArmazemDestino;
        oss << tipo;
    } else {
        throw std::invalid_argument("Evento inválido: deve ter pacote válido ou armazéns válidos.");
    }
    this->_chave = oss.str();
}

inline std::string Evento::getData() const {
    return this->_chave;
}

inline int Evento::getTempo() const {
    return this->_tempo;
}

inline int Evento::getIdPacote() const {
    return std::stoi(this->_chave.substr(6, 6));
}

inline Vetor<int> Evento::getArmazens() const {
    TipoEvento tipo = this->getTipoEvento();
    if (tipo != TipoEvento::TRANSPORTE) {
        throw std::logic_error("Evento não possui armazéns de origem e destino.");
    }
    Vetor<int> armazens(2);
    armazens[0] = std::stoi(this->_chave.substr(6, 3));
    armazens[1] = std::stoi(this->_chave.substr(9, 3));
    return armazens;
}

inline TipoEvento Evento::getTipoEvento() const {
    int tipo = std::stoi(this->_chave.substr(this->_chave.length() - 1, 1));
    switch (tipo) {
        case 1: return TipoEvento::CHEGADA_PACOTE;
        case 2: return TipoEvento::TRANSPORTE;
        default: throw std::logic_error("Tipo de evento desconhecido.");
    }
}

inline bool Evento::operator<(const Evento& other) const {
    return this->_chave < other._chave;
}

inline bool Evento::operator>(const Evento& other) const {
    return this->_chave > other._chave;
}

inline bool Evento::operator==(const Evento& other) const {
    return this->_chave == other._chave;
}

inline bool Evento::operator!=(const Evento& other) const {
    return this->_chave != other._chave;
}

inline bool Evento::operator<=(const Evento& other) const {
    return this->_chave <= other._chave;
}

inline bool Evento::operator>=(const Evento& other) const {
    return this->_chave >= other._chave;
}

#endif