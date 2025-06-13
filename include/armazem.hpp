#ifndef ARMAZEM_HPP
#define ARMAZEM_HPP

#include <stdexcept>
#include <iomanip>
#include <iostream>
#include "lista.hpp"
#include "vetor.hpp"
#include "pilha.hpp"
#include "pacote.hpp"

class Armazem {
    private:
        int _id;
        Lista<int> _vizinhos;
        Lista<Pilha<int>> _pacotesPorVizinho;
        Lista<Lista<int>> _transportesPorVizinho;
        Lista<int> _cooldownsPorVizinho;
        Lista<int> _capacidadesPorVizinho;

        int buscaVizinho(int id) const;

    public:
        Armazem();
        Armazem(int id);
        
        // Copy constructor and assignment operator
        Armazem(const Armazem& other);
        Armazem& operator=(const Armazem& other);
        
        void adicionaVizinho(int idVizinho);
        void removeVizinho(int idVizinho);

        void armazenaPacote(int idVizinho, int idPacote);
        int removePacotePorSecao(int idVizinho, int idPacote);
        
        // Agora retorna os pacotes a serem rearmazenados
        Pilha<int> adicionaPacotesParaTransporte(int idVizinho, int& tempoAtual, int custoRemocao);
        void rearmazenarPacotes(int idVizinho, Pilha<int> pacotes, int tempoAtual);
        Lista<int> getTransportesPorVizinho(int idVizinho);

        bool temPacotesArmazenados();
        
        int getId() const;
        int getCooldown(int idVizinho);
        Lista<int> getVizinhos() const;

        void setId(int id);
        void setCooldown(int idVizinho, int cooldown);
        void setCapacidade(int idVizinho, int capacidade);
};

// Implementation

inline Armazem::Armazem() : _id(-1) {}

inline Armazem::Armazem(int id) : _id(id) {}

inline Armazem::Armazem(const Armazem& other)
    : _id(other._id), 
      _vizinhos(other._vizinhos), 
      _pacotesPorVizinho(other._pacotesPorVizinho),
      _transportesPorVizinho(other._transportesPorVizinho),
      _cooldownsPorVizinho(other._cooldownsPorVizinho),
      _capacidadesPorVizinho(other._capacidadesPorVizinho) {}

inline Armazem& Armazem::operator=(const Armazem& other) {
    if (this != &other) {
        this->_id = other._id;
        this->_vizinhos = other._vizinhos;
        this->_pacotesPorVizinho = other._pacotesPorVizinho;
        this->_transportesPorVizinho = other._transportesPorVizinho;
        this->_cooldownsPorVizinho = other._cooldownsPorVizinho;
        this->_capacidadesPorVizinho = other._capacidadesPorVizinho;
    }
    return *this;
}

inline int Armazem::buscaVizinho(int id) const {
    Node<int>* aux = this->_vizinhos._head->GetNext();
    int pos = 1;
    while (aux != nullptr) {
        if (aux->GetData() == id) {
            return pos;
        }
        aux = aux->GetNext();
        pos++;
    }
    return -1;
}

inline void Armazem::adicionaVizinho(int vizinho) {
    Pilha<int> pilhaAux;
    Lista<int> listaAux;
    this->_vizinhos.InsereFim(vizinho);
    this->_pacotesPorVizinho.InsereFim(pilhaAux);
    this->_transportesPorVizinho.InsereFim(listaAux);
}

inline void Armazem::removeVizinho(int id) {
    int pos = this->buscaVizinho(id);
    this->_vizinhos.RemovePos(pos);
    this->_pacotesPorVizinho.RemovePos(pos);
}

inline Lista<int> Armazem::getVizinhos() const {
    return this->_vizinhos;
}  

inline void Armazem::armazenaPacote(int idVizinho, int idPacote) {
    int pos = this->buscaVizinho(idVizinho);
    if (pos == -1) {
        throw std::runtime_error("Vizinho não encontrado.");
    }
    this->_pacotesPorVizinho.Posiciona(pos)->GetDataRef().Empilha(idPacote);
}

inline int Armazem::removePacotePorSecao(int idVizinho, int idPacote) {
    int pos = this->buscaVizinho(idVizinho);
    if (pos == -1) {
        throw std::runtime_error("Vizinho não encontrado.");
    }
    if (this->_pacotesPorVizinho.Posiciona(pos)->GetData().Vazia()) {
        throw std::runtime_error("Nenhum pacote para enviar.");
    }
    Pilha<int> pilhaAux;
    int curId = this->_pacotesPorVizinho.Posiciona(pos)->GetData().Topo();
    while (curId != idPacote) {
        if (this->_pacotesPorVizinho.Posiciona(pos)->GetData().Vazia()) {
            throw std::runtime_error("Pacote não encontrado.");
        }
        pilhaAux.Empilha(
            this->_pacotesPorVizinho.Posiciona(pos)->GetData().Desempilha());
        curId = this->_pacotesPorVizinho.Posiciona(pos)->GetData().Topo();
    }
    while (!pilhaAux.Vazia()) {
        this->_pacotesPorVizinho
            .Posiciona(pos)->GetData().Empilha(pilhaAux.Desempilha());
    }
    return this->_pacotesPorVizinho.Posiciona(pos)->GetData().Desempilha();
}

inline Pilha<int> Armazem::adicionaPacotesParaTransporte(int idVizinho, int& tempoAtual, int custoRemocao) {
    int pos = this->buscaVizinho(idVizinho);
    if (pos == -1) {
        throw std::runtime_error("Vizinho não encontrado.");
    }
    int capacidade = this->_capacidadesPorVizinho.Posiciona(pos)->GetData();
    Lista<int>& transporte = this->_transportesPorVizinho.Posiciona(pos)->GetDataRef();
    Pilha<int>& pilhaPacotes = this->_pacotesPorVizinho.Posiciona(pos)->GetDataRef();
    while (transporte.GetTam() > 0) {
        transporte.RemovePos(1);
    }
    Pilha<int> pilhaAux;
    int count = 0;
    // Desempilha até esvaziar, colocando em pilha auxiliar
    while (!pilhaPacotes.Vazia()) {
        pilhaAux.Empilha(pilhaPacotes.Desempilha());
        tempoAtual += custoRemocao;
        std::cout << std::setfill('0')
                  << std::setw(7) << tempoAtual << " pacote "
                  << std::setw(3) << pilhaAux.Topo() << " removido de "
                  << std::setw(3) << this->_id - 1 << " na secao "
                  << std::setw(3) << idVizinho - 1 << std::endl;
    }
    // Agora desempilha da pilha auxiliar e insere no transporte (do fundo para o topo original)
    while (count < capacidade && !pilhaAux.Vazia()) {
        int pacote = pilhaAux.Desempilha();
        count++;
        transporte.InsereFim(pacote);
    }
    return pilhaAux;
}

inline void Armazem::rearmazenarPacotes(int idVizinho, Pilha<int> pacotes, int tempoAtual) {
    int pos = this->buscaVizinho(idVizinho);
    if (pos == -1) {
        throw std::runtime_error("Vizinho não encontrado.");
    }
    Pilha<int>& pilhaPacotes = this->_pacotesPorVizinho.Posiciona(pos)->GetDataRef();
    while (!pacotes.Vazia()) {
        int pacote = pacotes.Desempilha();
        pilhaPacotes.Empilha(pacote);
        std::cout << std::setfill('0')
                  << std::setw(7) << tempoAtual << " pacote "
                  << std::setw(3) << pacote << " rearmazenado em "
                  << std::setw(3) << this->_id - 1 << " na secao "
                  << std::setw(3) << idVizinho - 1 << std::endl;
    } 
}

inline Lista<int> Armazem::getTransportesPorVizinho(int idVizinho) {
    int pos = this->buscaVizinho(idVizinho);
    if (pos == -1) {
        throw std::runtime_error("Vizinho não encontrado.");
    }
    return this->_transportesPorVizinho.Posiciona(pos)->GetData();
}

inline bool Armazem::temPacotesArmazenados() {
    for (int i = 1; i <= this->_vizinhos.GetTam(); ++i) {
        if (!this->_pacotesPorVizinho.Posiciona(i)->GetData().Vazia()) {
            return true;
        }
    }
    return false;
}

inline int Armazem::getCooldown(int idVizinho) {
    int pos = this->buscaVizinho(idVizinho);
    if (pos == -1) {
        throw std::runtime_error("Vizinho não encontrado.");
    }
    return this->_cooldownsPorVizinho.Posiciona(pos)->GetData();
}

inline void Armazem::setCooldown(int idVizinho, int cooldown) {
    int pos = this->buscaVizinho(idVizinho);
    if (pos == -1) {
        throw std::runtime_error("Vizinho não encontrado.");
    }
    if (this->_cooldownsPorVizinho.GetTam() < pos + 1) {
        this->_cooldownsPorVizinho.InsereFim(cooldown);
        return;
    }
    this->_cooldownsPorVizinho.Posiciona(pos)->SetData(cooldown);
}

inline int Armazem::getId() const {
    return this->_id;
}

inline void Armazem::setId(int id) {
    this->_id = id;
}

inline void Armazem::setCapacidade(int idVizinho, int capacidade) {
    int pos = this->buscaVizinho(idVizinho);
    if (pos == -1) {
        throw std::runtime_error("Vizinho não encontrado.");
    }
    if (this->_capacidadesPorVizinho.GetTam() < pos + 1) {
        this->_capacidadesPorVizinho.InsereFim(capacidade);
        return;
    }
    this->_capacidadesPorVizinho.Posiciona(pos)->SetData(capacidade);
}

#endif