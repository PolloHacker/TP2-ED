#pragma once

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
void Pacote<T>::removeArmazemAtualDaRota() {
    if (this->_rota.GetTam() > 0) {
        this->_rota.RemovePos(1);
    }
}

template <typename T>
int Pacote<T>::getProximaSecaoRota() {
    if (this->_rota.GetTam() > 1) {
        return this->_rota.GetElemPos(2)->GetData();
    }
    return -1;
}

template <typename T>
int Pacote<T>::getProximoArmazemRota() {
    if (this->_rota.GetTam() > 0) {
        return this->_rota.GetElemPos(1)->GetData();
    }
    return -1;
}

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
Lista<int> Pacote<T>::getRota() const {
    return this->_rota;
}

template <typename T>
int Pacote<T>::getIdArmazemAtual() const {
    return this->_idArmazemAtual;
}

template <typename T>
int Pacote<T>::getIdSecaoAtual() const {
    return this->_idSecaoAtual;
}

template <typename T>
Metricas Pacote<T>::getMetricas() const {
    return this->_metricas;
}

template <typename T>
EstadoPacote Pacote<T>::getEstado() const {
    return this->_estado;
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

template <typename T>
void Pacote<T>::setRota(const Lista<int>& rota) {
    this->_rota = rota;
}

template <typename T>
void Pacote<T>::setIdArmazemAtual(int idArmazem) {
    this->_idArmazemAtual = idArmazem;
}

template <typename T>
void Pacote<T>::setIdSecaoAtual(int idSecao) {
    this->_idSecaoAtual = idSecao;
}

template <typename T>
void Pacote<T>::setEstado(EstadoPacote estado) {
    this->_estado = estado;
}