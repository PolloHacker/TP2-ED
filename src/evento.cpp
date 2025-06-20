#include "evento.hpp"

Evento::Evento()
    : _chave(""), _tempo(-1) {}

Evento::Evento(std::string chave)
    : _chave(chave) {
    
    if (chave.length() < 20) {
        throw std::invalid_argument("Chave deve ter pelo menos 20 caracteres.");
    }

    this->_tempo = std::stoll(chave.substr(0, 13));
}

Evento::Evento(int tempo, int idPacote, int idArmazemOrigem, int idArmazemDestino, TipoEvento tipoEvento) 
    : _tempo(tempo) {
        
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

    // Always start with time (13 digits)
    oss << std::setw(13) << tempo;

    if (idPacote >= 0) {
        // Valid packet ID: time(13) + pacote(6) + tipo(1)
        oss << std::setw(6) << idPacote;
        oss << tipo;
    } else if (idArmazemOrigem >= 0 && idArmazemDestino >= 0) {
        // Invalid packet but valid warehouses: time(13) + origem(3) + destino(3) + tipo(1)
        oss << std::setw(3) << idArmazemOrigem;
        oss << std::setw(3) << idArmazemDestino;
        oss << tipo;
    } else {
        throw std::invalid_argument("Evento inválido: deve ter pacote válido ou armazéns válidos.");
    }

    this->_chave = oss.str();
}

std::string Evento::getData() const {
    return this->_chave;
}

int Evento::getTempo() const {
    return this->_tempo;
}

int Evento::getIdPacote() const {
    // idPacote is always present in the key, regardless of event type
    return std::stoi(this->_chave.substr(13, 6));
}

Vetor<int> Evento::getArmazens() const {
    // Only TRANSPORTE, REARMAZENAMENTO, and ENTREGA have both armazemOrigem and armazemDestino
    TipoEvento tipo = this->getTipoEvento();
    if (tipo != TipoEvento::TRANSPORTE) {
        throw std::logic_error("Evento não possui armazéns de origem e destino.");
    }

    Vetor<int> armazens(2);
    armazens[0] = std::stoi(this->_chave.substr(13, 3)) + 1; // armazemOrigem
    armazens[1] = std::stoi(this->_chave.substr(16, 3)) + 1; // armazemDestino
    return armazens;
}

TipoEvento Evento::getTipoEvento() const {
    // tipoEvento is always the last character of the key
    int tipo = std::stoi(this->_chave.substr(this->_chave.length() - 1, 1));
    switch (tipo) {
        case 1: return TipoEvento::CHEGADA_PACOTE;
        case 2: return TipoEvento::TRANSPORTE;
        default: throw std::logic_error("Tipo de evento desconhecido.");
    }
}

bool Evento::operator<(const Evento& other) const {
    return this->_chave < other._chave;
}

bool Evento::operator>(const Evento& other) const {
    return this->_chave > other._chave;
}

bool Evento::operator==(const Evento& other) const {
    return this->_chave == other._chave;
}

bool Evento::operator!=(const Evento& other) const {
    return this->_chave != other._chave;
}

bool Evento::operator<=(const Evento& other) const {
    return this->_chave <= other._chave;
}

bool Evento::operator>=(const Evento& other) const {
    return this->_chave >= other._chave;
}