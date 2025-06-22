#ifndef PACOTE_HPP
#define PACOTE_HPP

#include <string>
#include "lista.hpp"
#include "utils.hpp"

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
        int _ultimoTempoArmazenamento;
        int _tempoPostagem;
        EstadoPacote _estado;
           
    public:
        Pacote();
        Pacote(int id, const std::string& nome_remente, const std::string& nome_destinatario, const T& conteudo);

        int removeArmazemAtualDaRota();
        int getProximoArmazemRota();
        int getDestinoFinal();  // New method to get final destination (removed const)
        
        int getId() const;
        std::string getNomeRemetente() const;
        std::string getNomeDestinatario() const;
        T getConteudo() const;
        Lista<int> getRota() const;
        int getIdArmazemAtual() const;
        int getIdSecaoAtual() const;
        EstadoPacote getEstado() const;
        int getUltimoTempoArmazenamento() const;
        int getTempoPostagem() const;
        
        void setId(int id);
        void setNomeRemetente(const std::string& nome_remente);
        void setNomeDestinatario(const std::string& nome_destinatario);
        void setConteudo(const T& conteudo);
        void setRota(const Lista<int>& rota);
        void setIdArmazemAtual(int idArmazem);
        void setIdSecaoAtual(int idSecao);
        void setEstado(EstadoPacote estado);
        void setUltimoTempoArmazenamento(int tempo);
        void setTempoPostagem(int tempo);
};


/**
 * @brief Construtor padrão da classe Pacote.
 *
 * Inicializa um objeto Pacote com nomes de remetente e destinatário vazios,
 * e o conteúdo do tipo T inicializado com seu valor padrão.
 */
template <typename T>
Pacote<T>::Pacote() : _nome_remente(""), _nome_destinatario("") {}

/**
 * @brief Construtor da classe Pacote.
 *
 * Inicializa um objeto Pacote com o ID, nomes de remetente e destinatário,
 * e o conteúdo fornecido. O estado do pacote é definido como POR_POSTAR.
 *
 * @param id Identificador único do pacote.
 * @param nome_remente Nome do remetente do pacote.
 * @param nome_destinatario Nome do destinatário do pacote.
 * @param conteudo Conteúdo do pacote, do tipo T.
 */
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



/**
 * @brief Remove o armazém atual da rota do pacote.
 *
 * Esta função verifica se a rota do pacote possui pelo menos um armazém.
 * Caso positivo, remove o armazém na posição 1 da rota e retorna o valor removido.
 * Caso a rota esteja vazia, retorna -1.
 *
 * @return int Valor removido da rota ou -1 se a rota estiver vazia.
 */
template <typename T>
int Pacote<T>::removeArmazemAtualDaRota() {
    if (this->_rota.GetTam() > 0) {
        return this->_rota.RemovePos(1);
    }
    return -1;
}

/**
 * @brief Obtém o próximo armazém na rota do pacote.
 *
 * Esta função verifica se a rota do pacote possui pelo menos um armazém.
 * Se sim, retorna o valor do armazém na posição 1 da rota.
 * Caso contrário, retorna -1.
 *
 * @return int Identificador do próximo armazém na rota ou -1 se não houver armazéns.
 */
template <typename T>
int Pacote<T>::getProximoArmazemRota() {
    if (this->_rota.GetTam() > 0) {
        return this->_rota.GetElemPos(1)->GetData();
    }
    return -1;
}

/**
 * @brief Obtém o destino final do pacote.
 *
 * Este método retorna o identificador do último armazém na rota,
 * que representa o destino final do pacote.
 *
 * @return int ID do armazém de destino final ou -1 se a rota estiver vazia.
 */
template <typename T>
int Pacote<T>::getDestinoFinal() {
    if (this->_rota.GetTam() > 0) {
        return this->_rota.GetElemPos(this->_rota.GetTam())->GetData();
    }
    return -1;
}

/**
 * @brief Obtém o ID do pacote.
 *
 * Este método retorna o identificador único do pacote.
 *
 * @return int ID do pacote.
 */
template <typename T>
int Pacote<T>::getId() const {
    return this->_id;
}

/**
 * @brief Obtém o nome do remetente do pacote.
 *
 * Este método retorna o nome do remetente associado ao pacote.
 *
 * @return std::string Nome do remetente.
 */
template <typename T>
std::string Pacote<T>::getNomeRemetente() const {
    return this->_nome_remente;
}

/**
 * @brief Obtém o nome do destinatário do pacote.
 *
 * Este método retorna o nome do destinatário associado ao pacote.
 *
 * @return std::string Nome do destinatário.
 */
template <typename T>
std::string Pacote<T>::getNomeDestinatario() const {
    return this->_nome_destinatario;
}

/**
 * @brief Obtém o conteúdo do pacote.
 *
 * Este método retorna o conteúdo armazenado no pacote, do tipo T.
 *
 * @return T Conteúdo do pacote.
 */
template <typename T>
T Pacote<T>::getConteudo() const {
    return this->_conteudo;
}

/**
 * @brief Obtém a rota do pacote.
 *
 * Este método retorna a lista de armazéns que compõem a rota do pacote.
 *
 * @return Lista<int> Rota do pacote, representada por uma lista de IDs de armazéns.
 */
template <typename T>
Lista<int> Pacote<T>::getRota() const {
    return this->_rota;
}

/**
 * @brief Obtém o ID do armazém atual do pacote.
 *
 * Este método retorna o identificador do armazém onde o pacote está atualmente localizado.
 *
 * @return int ID do armazém atual do pacote.
 */
template <typename T>
int Pacote<T>::getIdArmazemAtual() const {
    return this->_idArmazemAtual;
}

/**
 * @brief Obtém o ID da seção atual do pacote.
 *
 * Este método retorna o identificador da seção onde o pacote está atualmente localizado.
 *
 * @return int ID da seção atual do pacote.
 */
template <typename T>
int Pacote<T>::getIdSecaoAtual() const {
    return this->_idSecaoAtual;
}

/**
 * @brief Obtém o estado atual do pacote.
 *
 * Este método retorna o estado do pacote, que pode ser POR_POSTAR, POSTADO, EM_SEPARACAO,
 * EM_TRANSPORTE ou ENTREGUE.
 *
 * @return EstadoPacote Estado atual do pacote.
 */
template <typename T>
EstadoPacote Pacote<T>::getEstado() const {
    return this->_estado;
}

/**
 * @brief Obtém o último tempo de armazenamento do pacote.
 *
 * @return int Último tempo de armazenamento do pacote.
 */
template <typename T>
int Pacote<T>::getUltimoTempoArmazenamento() const {
    return this->_ultimoTempoArmazenamento;
}


/**
 * @brief Obtém o tempo de postagem do pacote.
 *
 * @return int Tempo de postagem do pacote.
 */
template <typename T>
int Pacote<T>::getTempoPostagem() const {
    return this->_tempoPostagem;
}

/**
 * @brief Define o ID do pacote.
 *
 * Este método atualiza o identificador único do pacote.
 *
 * @param id Novo ID do pacote.
 */
template <typename T>
void Pacote<T>::setId(int id) {
    this->_id = id;
}

/**
 * @brief Define o nome do remetente do pacote.
 *
 * Este método atualiza o nome do remetente associado ao pacote.
 *
 * @param nome_remente Novo nome do remetente.
 */
template <typename T>
void Pacote<T>::setNomeRemetente(const std::string& nome_remente) {
    this->_nome_remente = nome_remente;
}

/**
 * @brief Define o nome do destinatário do pacote.
 *
 * Este método atualiza o nome do destinatário associado ao pacote.
 *
 * @param nome_destinatario Novo nome do destinatário.
 */
template <typename T>
void Pacote<T>::setNomeDestinatario(const std::string& nome_destinatario) {
    this->_nome_destinatario = nome_destinatario;
}

/**
 * @brief Define o conteúdo do pacote.
 *
 * Este método atualiza o conteúdo armazenado no pacote, do tipo T.
 *
 * @param conteudo Novo conteúdo do pacote.
 */
template <typename T>
void Pacote<T>::setConteudo(const T& conteudo) {
    this->_conteudo = conteudo;
}

/**
 * @brief Define a rota do pacote.
 *
 * Este método atualiza a lista de armazéns que compõem a rota do pacote.
 *
 * @param rota Lista de IDs de armazéns que formam a nova rota do pacote.
 */
template <typename T>
void Pacote<T>::setRota(const Lista<int>& rota) {
    this->_rota = rota;
}

/**
 * @brief Define o ID do armazém atual do pacote.
 *
 * Este método atualiza o identificador do armazém onde o pacote está atualmente localizado.
 *
 * @param idArmazem Novo ID do armazém atual do pacote.
 */
template <typename T>
void Pacote<T>::setIdArmazemAtual(int idArmazem) {
    this->_idArmazemAtual = idArmazem;
}

/**
 * @brief Define o ID da seção atual do pacote.
 *
 * Este método atualiza o identificador da seção onde o pacote está atualmente localizado.
 *
 * @param idSecao Novo ID da seção atual do pacote.
 */
template <typename T>
void Pacote<T>::setIdSecaoAtual(int idSecao) {
    this->_idSecaoAtual = idSecao;
}

/**
 * @brief Define o estado atual do pacote.
 *
 * Este método atualiza o estado do pacote, que pode ser POR_POSTAR, POSTADO, EM_SEPARACAO,
 * EM_TRANSPORTE ou ENTREGUE.
 *
 * @param estado Novo estado do pacote.
 */
template <typename T>
void Pacote<T>::setEstado(EstadoPacote estado) {
    this->_estado = estado;
}

/**
 * @brief Define o último tempo de armazenamento do pacote.
 *
 * Este método atualiza o último tempo em que o pacote foi armazenado.
 *
 * @param tempo Novo tempo de armazenamento do pacote.
 */
template <typename T>
void Pacote<T>::setUltimoTempoArmazenamento(int tempo) {
    this->_ultimoTempoArmazenamento = tempo;
}

/**
 * @brief Define o tempo de postagem do pacote.
 *
 * Este método atualiza o tempo em que o pacote foi postado.
 *
 * @param tempo Novo tempo de postagem do pacote.
 */
template <typename T>
void Pacote<T>::setTempoPostagem(int tempo) {
    this->_tempoPostagem = tempo;
}

#endif