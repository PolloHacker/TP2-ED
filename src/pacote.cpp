#include "pacote.hpp"

template <typename T>
Pacote<T>::Pacote() : _nome_remente(""), _nome_destinatario(""), _conteudo(T()) {}

template <typename T>
Pacote<T>::Pacote(
    int id,
    const std::string &nome_remente,
    const std::string &nome_destinatario,
    const T &conteudo)
    : _id(id),
      _nome_remente(nome_remente),
      _nome_destinatario(nome_destinatario),
      _conteudo(conteudo),
      _estado(EstadoPacote::POR_POSTAR) {}


template <typename T>
int Pacote<T>::getId() const {
    return this->_id;
}

template <typename T>
std::string Pacote<T>::getNomeRemetente() const {
    return this->_nome_remente;
}

template <typename T>
std::string Pacote<T>::getNomeDestinatario() const {
    return this->_nome_destinatario;
}

template <typename T>
T Pacote<T>::getConteudo() const {
    return this->_conteudo;
}

template <typename T>
void Pacote<T>::setId(int id) {
    this->_id = id;
}

template <typename T>
void Pacote<T>::setNomeRemetente(const std::string& nome_remente) {
    this->_nome_remente = nome_remente;
}

template <typename T>
void Pacote<T>::setNomeDestinatario(const std::string& nome_destinatario) {
    this->_nome_destinatario = nome_destinatario;
}

template <typename T>
void Pacote<T>::setConteudo(const T& conteudo) {
    this->_conteudo = conteudo;
}

template class Pacote<std::string>;