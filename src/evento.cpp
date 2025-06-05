#include "evento.hpp"

Evento::Evento()
    : _id(-1), _tempo(-1), _tipo(TipoEvento::POR_POSTAR) {}

Evento::Evento(int tempo, int id, const TipoEvento& estado)
    : _id(id), _tempo(tempo), _tipo(estado) {}

double Evento::getTempo() const {
    return _tempo;
}

int Evento::getId() const {
    return _id;
}

TipoEvento Evento::getTipo() const {
    return _tipo;
}

void Evento::setTempo(double tempo) {
    _tempo = tempo;
}

void Evento::setId(int id) {
    _id = id;
}

void Evento::setTipo(const TipoEvento& estado) {
    _tipo = estado;
}

bool Evento::operator<(const Evento& other) const {
    return _tempo < other._tempo;
}

bool Evento::operator>(const Evento& other) const {
    return _tempo > other._tempo;
}

bool Evento::operator==(const Evento& other) const {
    return _id == other._id && _tempo == other._tempo && _tipo == other._tipo;
}

bool Evento::operator!=(const Evento& other) const {
    return !(*this == other);
}

bool Evento::operator<=(const Evento& other) const {
    return _tempo <= other._tempo;
}

bool Evento::operator>=(const Evento& other) const {
    return _tempo >= other._tempo;
}