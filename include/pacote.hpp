#ifndef PACOTE_HPP
#define PACOTE_HPP

#include <string>
#include "armazem.hpp"

enum class EstadoTransporte {
    POR_POSTAR,
    POSTADO,
    POR_ARMAZENAR,
    ARMAZENADO,
    EM_ROTA,
    ENTREGUE
};

template <typename T>
class Pacote {
    private:
        std::string _nome_remente;
        std::string _nome_destinatario;
        T _conteudo;
        EstadoTransporte _estado;
           
    public:
        Pacote();
        Pacote(const std::string& nome_remente, const std::string& nome_destinatario, const T& conteudo);
        
        std::string getNomeRemetente() const;
        std::string getNomeDestinatario() const;
        T getConteudo() const;
        
        void setNomeRemetente(const std::string& nome_remente);
        void setNomeDestinatario(const std::string& nome_destinatario);
        void setConteudo(const T& conteudo);
};

#endif