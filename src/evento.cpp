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

Evento::Evento(int tempo, int idPacote, TipoEvento tipoEvento)
    : _tempo(tempo) {
    
    if (tipoEvento != TipoEvento::PACOTE) {
        throw std::invalid_argument("Tipo de evento deve ser PACOTE.");
    }

    this->_chave = std::string(6 - std::to_string(tempo).length(), '0') +
                   std::to_string(tempo) +
                   std::string(6 - std::to_string(idPacote).length(), '0') +
                   std::to_string(idPacote) + "1";
}
Evento::Evento(int tempo, int idArmazemOrigem, int idArmazemDestino, TipoEvento tipoEvento)
    : _tempo(tempo) {
    
    if (tipoEvento != TipoEvento::TRANSPORTE) {
        throw std::invalid_argument("Tipo de evento deve ser TRANSPORTE.");
    }

    this->_chave = std::string(6 - std::to_string(tempo).length(), '0') +
                   std::to_string(tempo) +
                   std::string(3 - std::to_string(idArmazemOrigem).length(), '0') +
                   std::to_string(idArmazemOrigem) +
                   std::string(3 - std::to_string(idArmazemDestino).length(), '0') +
                   std::to_string(idArmazemDestino) + "2";
}


std::string Evento::getData() const {
    return this->_chave;
}

int Evento::getTempo() const {
    return this->_tempo;
}

int Evento::getIdPacote() const {
    if (this->getTipoEvento() != TipoEvento::PACOTE) {
        throw std::logic_error("Evento não é do tipo PACOTE.");
    }

    return std::stoi(this->_chave.substr(3, 6));
}

Vetor<int> Evento::getArmazens() const {
    if (this->getTipoEvento() != TipoEvento::TRANSPORTE) {
        throw std::logic_error("Evento não é do tipo TRANSPORTE.");
    }

    Vetor<int> armazens(2);

    armazens[0] = std::stoi(this->_chave.substr(6, 3));
    armazens[1] = std::stoi(this->_chave.substr(9, 3));
    
    return armazens;
}

TipoEvento Evento::getTipoEvento() const {
    return std::stoi(this->_chave.substr(12, 1)) == 1 ? 
        TipoEvento::PACOTE : TipoEvento::TRANSPORTE;
}

bool Evento::operator<(const Evento& other) const {
    return this->_tempo < other._tempo;
}

bool Evento::operator>(const Evento& other) const {
    return this->_tempo > other._tempo;
}

bool Evento::operator==(const Evento& other) const {
    return this->_chave == other._chave;
}

bool Evento::operator!=(const Evento& other) const {
    return !(*this == other);
}

bool Evento::operator<=(const Evento& other) const {
    return this->_tempo <= other._tempo;
}

bool Evento::operator>=(const Evento& other) const {
    return this->_tempo >= other._tempo;
}