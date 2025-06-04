#include "pacote.hpp"

template <typename T>
Pacote<T>::Pacote() : _nome_remente(""), _nome_destinatario(""), _conteudo(T()) {
    this->_estado = EstadoTransporte::POR_POSTAR;
}

template <typename T>
Pacote<T>::Pacote(const std::string& nome_remente, const std::string& nome_destinatario, const T& conteudo)
    : _nome_remente(nome_remente), _nome_destinatario(nome_destinatario), _conteudo(conteudo) {}

template <typename T>
std::string Pacote<T>::getNomeRemetente() const {
    return _nome_remente;
}

template <typename T>
std::string Pacote<T>::getNomeDestinatario() const {
    return _nome_destinatario;
}

template <typename T>
T Pacote<T>::getConteudo() const {
    return _conteudo;
}

template <typename T>
void Pacote<T>::setNomeRemetente(const std::string& nome_remente) {
    _nome_remente = nome_remente;
}

template <typename T>
void Pacote<T>::setNomeDestinatario(const std::string& nome_destinatario) {
    _nome_destinatario = nome_destinatario;
}

template <typename T>
void Pacote<T>::setConteudo(const T& conteudo) {
    _conteudo = conteudo;
}

template class Pacote<std::string>;