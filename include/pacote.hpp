#ifndef PACOTE_HPP
#define PACOTE_HPP

#include <string>
#include "lista.hpp"
#include "metricas.hpp"

enum class EstadoPacote {
    POR_POSTAR, // Não foi postado
    POSTADO, // Chegada escalonada a um armazém (postagem ou transporte)
    EM_SEPARACAO, // Armazenado na seção associada ao próximo destino de um armazém
    EM_TRANSPORTE, // Removido da seção para transporte
    ENTREGUE // Entregue
};

template <typename T>
class Pacote {
    private:
        int _id;
        std::string _nome_remente;
        std::string _nome_destinatario;
        T _conteudo;
        Lista<int> _rota;
        int _idArmazemAtual;
        int _idSecaoAtual;
        Metricas _metricas;
        EstadoPacote _estado;
    public:
        Pacote();
        Pacote(int id, const std::string& nome_remente, const std::string& nome_destinatario, const T& conteudo);
        int removeArmazemAtualDaRota();
        int getProximoArmazemRota();
        int getId() const;
        std::string getNomeRemetente() const;
        std::string getNomeDestinatario() const;
        T getConteudo() const;
        Lista<int> getRota() const;
        int getIdArmazemAtual() const;
        int getIdSecaoAtual() const;
        Metricas getMetricas() const;
        EstadoPacote getEstado() const;
        void setId(int id);
        void setNomeRemetente(const std::string& nome_remente);
        void setNomeDestinatario(const std::string& nome_destinatario);
        void setConteudo(const T& conteudo);
        void setRota(const Lista<int>& rota);
        void setIdArmazemAtual(int idArmazem);
        void setIdSecaoAtual(int idSecao);
        void setEstado(EstadoPacote estado);
};

// Implementation

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
int Pacote<T>::removeArmazemAtualDaRota() {
    if (this->_rota.GetTam() > 0) {
        return this->_rota.RemovePos(1);
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

#endif