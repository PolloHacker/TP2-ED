#include "evento.hpp"

Evento::Evento()
    : _chave(""), _tempo(-1) {}

Evento::Evento(std::string chave)
    : _chave(chave) {
    
    if (chave.length() < 13) {
        throw std::invalid_argument("Chave deve ter pelo menos 13 caracteres.");
    }

    this->_tempo = std::stoi(chave.substr(0, 6));
    printf("Chave: %s\n", this->_chave.c_str());
}

Evento::Evento(int tempo, int idPacote, int idArmazemOrigem, int idArmazemDestino, TipoEvento tipoEvento) 
    : _tempo(tempo) {
        
    int tipo = 0;
    switch (tipoEvento) {
        case TipoEvento::CHEGADA_PACOTE: tipo = 1; break;
        case TipoEvento::TRANSPORTE: tipo = 2; break;

        default:
            throw std::invalid_argument("Tipo de evento inválido.");
    }

    this->_chave = std::string(6 - std::to_string(tempo).length(), '0') +
                   std::to_string(tempo) +
                   std::string(6 - std::to_string(idPacote).length(), '0') +
                   std::to_string(idPacote) +
                   std::string(3 - std::to_string(idArmazemOrigem).length(), '0') +
                   std::to_string(idArmazemOrigem) +
                   std::string(3 - std::to_string(idArmazemDestino).length(), '0') +
                   std::to_string(idArmazemDestino) + 
                   std::to_string(tipo);
}

std::string Evento::getData() const {
    return this->_chave;
}

int Evento::getTempo() const {
    return this->_tempo;
}

int Evento::getIdPacote() const {
    // idPacote is always present in the key, regardless of event type
    return std::stoi(this->_chave.substr(6, 6));
}

Vetor<int> Evento::getArmazens() const {
    // Only TRANSPORTE, REARMAZENAMENTO, and ENTREGA have both armazemOrigem and armazemDestino
    TipoEvento tipo = this->getTipoEvento();
    if (tipo != TipoEvento::TRANSPORTE) {
        throw std::logic_error("Evento não possui armazéns de origem e destino.");
    }

    Vetor<int> armazens(2);
    armazens[0] = std::stoi(this->_chave.substr(12, 3)); // armazemOrigem
    armazens[1] = std::stoi(this->_chave.substr(15, 3)); // armazemDestino
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
    if (this->_tempo != other._tempo)
        return this->_tempo < other._tempo;

    // Compare armazemOrigem
    int thisOrigem = 0, otherOrigem = 0;
    try {
        thisOrigem = std::stoi(this->_chave.substr(12, 3));
        otherOrigem = std::stoi(other._chave.substr(12, 3));
    } catch (...) {
        // If not TRANSPORTE, fallback to 0
    }
    if (thisOrigem != otherOrigem)
        return thisOrigem < otherOrigem;

    // Compare armazemDestino
    int thisDestino = 0, otherDestino = 0;
    try {
        thisDestino = std::stoi(this->_chave.substr(15, 3));
        otherDestino = std::stoi(other._chave.substr(15, 3));
    } catch (...) {
        // If not TRANSPORTE, fallback to 0
    }
    return thisDestino < otherDestino;
}

bool Evento::operator>(const Evento& other) const {
    return other < *this;
}

bool Evento::operator==(const Evento& other) const {
    return this->_chave == other._chave;
}

bool Evento::operator!=(const Evento& other) const {
    return !(*this == other);
}

bool Evento::operator<=(const Evento& other) const {
    return !(other < *this);
}

bool Evento::operator>=(const Evento& other) const {
    return !(*this < other);
}