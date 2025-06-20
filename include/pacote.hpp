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
        int _ultimoTempoArmazenamento;
        int _tempoPostagem;
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
        EstadoPacote getEstado() const;
        int getLastStorageTime() const;
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

#include "pacote.tpp"

#endif