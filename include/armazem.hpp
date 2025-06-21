#ifndef ARMAZEM_HPP
#define ARMAZEM_HPP

#include <stdexcept>
#include <iomanip>
#include <iostream>
#include "lista.hpp"
#include "vetor.hpp"
#include "pilha.hpp"
#include "hashTable.hpp"
#include "pacote.hpp"


struct Vizinho {
    Pilha<int> pacotes;
    Lista<int> transportes;
    int cooldown;
    int capacidade;
};

class Armazem {
    private:
        int _id;
        Lista<int> _vizinhos;
        HashTable<Vizinho> _dadosVizinho;

    public:
        Armazem();
        Armazem(int id);

        Armazem(const Armazem& other);
        Armazem& operator=(const Armazem& other);
        
        void adicionaVizinho(int idVizinho);

        void armazenaPacote(int idVizinho, int idPacote, Metricas& metricas);
        int removePacotePorSecao(int idVizinho, int idPacote);
        
        Pilha<int> adicionaPacotesParaTransporte(int idVizinho, int& tempoAtual, int custoRemocao, Metricas& metricas);
        void rearmazenarPacotes(int idVizinho, Pilha<int> pacotes, int tempoAtual, Metricas& metricas);
        Lista<int> getTransportesPorVizinho(int idVizinho);
        
        int getId() const;
        int getCooldown(int idVizinho);
        Lista<int> getVizinhos() const;

        void setId(int id);
        void setCooldown(int idVizinho, int cooldown);        
        void setCapacidade(int idVizinho, int capacidade);
};

/**
 * @brief Construtor padrão da classe Armazem.
 *
 * Inicializa o objeto Armazem com o identificador (_id) igual a -1,
 * indicando que o armazém ainda não possui um ID válido atribuído.
 */
Armazem::Armazem() : _id(-1) {}

/**
 * @brief Construtor da classe Armazem.
 * 
 * Inicializa um novo objeto Armazem com o identificador fornecido.
 * 
 * @param id Identificador único do armazém.
 */
Armazem::Armazem(int id) : _id(id) {}

/**
 * @brief Construtor de cópia da classe Armazem.
 *
 * Cria uma nova instância de Armazem copiando os valores dos atributos
 * de outro objeto Armazem fornecido como parâmetro.
 *
 * @param other Referência constante para o objeto Armazem a ser copiado.
 */
Armazem::Armazem(const Armazem& other)
    : _id(other._id), 
      _vizinhos(other._vizinhos), 
      _dadosVizinho(other._dadosVizinho) {}

/**
 * @brief Operador de atribuição para a classe Armazem.
 *
 * Copia os valores dos atributos de outro objeto Armazem para o objeto atual,
 * garantindo que não haja autoatribuição. Os atributos copiados incluem _id,
 * _vizinhos e _dadosVizinho.
 *
 * @param other Referência constante para o objeto Armazem a ser copiado.
 * @return Referência para o próprio objeto Armazem após a atribuição.
 */
Armazem& Armazem::operator=(const Armazem& other) {
    if (this != &other) {
        this->_id = other._id;
        this->_vizinhos = other._vizinhos;
        this->_dadosVizinho = other._dadosVizinho;
    }
    return *this;
}


/**
 * @brief Adiciona um novo vizinho ao armazém.
 *
 * Esta função insere o identificador do vizinho na lista de vizinhos do armazém
 * e inicializa os dados associados a esse vizinho.
 *
 * @param vizinho Identificador do armazém vizinho a ser adicionado.
 */
void Armazem::adicionaVizinho(int vizinho) {
    this->_vizinhos.InsereFim(vizinho);

    Vizinho novoVizinho;
    this->_dadosVizinho.insere(vizinho, novoVizinho);
}

/**
 * @brief Retorna a lista de vizinhos do armazém.
 *
 * Este método fornece acesso à lista de armazéns vizinhos associados a esta instância.
 *
 * @return Lista<int> contendo os identificadores dos armazéns vizinhos.
 */
Lista<int> Armazem::getVizinhos() const {
    return this->_vizinhos;
}  

/**
 * @brief Armazena um pacote na pilha de pacotes de um vizinho específico.
 *
 * Esta função localiza o vizinho pelo seu identificador e adiciona o pacote,
 * identificado por idPacote, à pilha de pacotes desse vizinho.
 * 
 * @param idVizinho Identificador do vizinho onde o pacote será armazenado.
 * @param idPacote Identificador do pacote a ser armazenado.
 * 
 * @throws std::runtime_error Caso o vizinho com o id especificado não seja encontrado.
 */
void Armazem::armazenaPacote(int idVizinho, int idPacote, Metricas& metricas) {
    Vizinho* vizinho = this->_dadosVizinho.getValor(idVizinho);

    if (vizinho == nullptr) {
        throw std::runtime_error("Vizinho não encontrado.");
    }

    vizinho->pacotes.Empilha(idPacote);

    metricas.incStackPush();
    metricas.sampleSectionDepth(vizinho->pacotes.GetTam());
}

/**
 * @brief Remove um pacote específico da pilha de pacotes de um vizinho.
 *
 * @param idVizinho Identificador do vizinho de onde o pacote será removido.
 * @param idPacote Identificador do pacote a ser removido.
 * @return int O identificador do pacote removido.
 * @throws std::runtime_error Se o vizinho não for encontrado, se não houver pacotes para enviar,
 *         ou se o pacote não for encontrado na pilha.
 */
int Armazem::removePacotePorSecao(int idVizinho, int idPacote) {
    Vizinho* vizinho = this->_dadosVizinho.getValor(idVizinho);

    // Verifica se o vizinho existe
    if (vizinho == nullptr) {
        throw std::runtime_error("Vizinho não encontrado.");
    }

    // Verifica se a pilha de pacotes do vizinho está vazia
    if (vizinho->pacotes.Vazia()) {
        throw std::runtime_error("Nenhum pacote para enviar.");
    }

    Pilha<int> pilhaAux;
    Pilha<int>& pilhaPacotes = vizinho->pacotes;

    int curId = pilhaPacotes.Topo();

    /*
    * Percorre a pilha de pacotes até encontrar o pacote com o idPacote.
    * Durante o percurso, empilha os pacotes em uma pilha auxiliar.
    */
    while (curId != idPacote) {
        if (pilhaPacotes.Vazia()) {
            throw std::runtime_error("Pacote não encontrado.");
        }

        pilhaAux.Empilha(pilhaPacotes.Desempilha());
        if (pilhaPacotes.Vazia()) {
            throw std::runtime_error("Pacote não encontrado.");
        }
        curId = pilhaPacotes.Topo();
    }
    
    // Desempilha os pacotes restantes da pilha de pacotes do vizinho
    while (!pilhaAux.Vazia()) {
        pilhaPacotes.Empilha(pilhaAux.Desempilha());
    }

    // Desempilha o pacote com o idPacote
    return pilhaPacotes.Desempilha();
}

/**
 * @brief Adiciona pacotes do armazém para transporte a um vizinho específico.
 *
 * @param idVizinho Identificador do vizinho para o qual os pacotes serão transportados.
 * @param tempoAtual Referência para o tempo atual, que será incrementado pelo custo de remoção de cada pacote.
 * @param custoRemocao Custo de tempo para remover cada pacote da pilha.
 * @return Pilha<int> Pilha auxiliar contendo os pacotes que não foram transportados devido à limitação de capacidade.
 *
 * @throws std::runtime_error Caso o vizinho não seja encontrado.
 */
Pilha<int> Armazem::adicionaPacotesParaTransporte(
    int idVizinho, int& tempoAtual, int custoRemocao, Metricas& metricas) {
    
    Vizinho* vizinho = this->_dadosVizinho.getValor(idVizinho);

    // Verifica se o vizinho existe
    if (vizinho == nullptr) {
        throw std::runtime_error("Vizinho não encontrado.");
    }

    int capacidade = vizinho->capacidade;
    Lista<int>& transporte = vizinho->transportes;
    Pilha<int>& pilhaPacotes = vizinho->pacotes;

    // Limpa a lista antiga de transportes do vizinho
    while (transporte.GetTam() > 0) {
        transporte.RemovePos(1);
    }

    Pilha<int> pilhaAux;
    int count = 0;

    /* 
    * Enquanto houver pacotes na pilha e a capacidade não for atingida,
    * desempilha os pacotes e adiciona na lista de transportes.
    */
    while (!pilhaPacotes.Vazia()) {
        pilhaAux.Empilha(pilhaPacotes.Desempilha());
        tempoAtual += custoRemocao;

        metricas.incPackagesMoved();
        metricas.incStackPop();
        metricas.sampleSectionDepth(pilhaPacotes.GetTam());

        std::cout << std::setfill('0')
                  << std::setw(7) << tempoAtual << " pacote "
                  << std::setw(3) << pilhaAux.Topo() << " removido de "
                  << std::setw(3) << this->_id - 1 << " na secao "
                  << std::setw(3) << idVizinho - 1 << std::endl;
    }

    while (count < capacidade && !pilhaAux.Vazia()) {
        int pacote = pilhaAux.Desempilha();
        count++;
        transporte.InsereFim(pacote);
    }

    return pilhaAux;
}

/**
 * @brief Rearmazena pacotes de um vizinho no armazém.
 *
 * @param idVizinho Identificador do vizinho para o qual os pacotes serão rearmazenados.
 * @param pacotes Pilha de pacotes a serem transferidos para o vizinho.
 * @param tempoAtual Tempo atual no qual a operação está sendo realizada.
 *
 * @throws std::runtime_error Caso o vizinho não seja encontrado.
 */
void Armazem::rearmazenarPacotes(
    int idVizinho, Pilha<int> pacotes, int tempoAtual, Metricas& metricas) {
    
    Vizinho* vizinho = this->_dadosVizinho.getValor(idVizinho);

    // Verifica se o vizinho existe
    if (vizinho == nullptr) {
        throw std::runtime_error("Vizinho não encontrado.");
    }

    Pilha<int>& pilhaPacotes = vizinho->pacotes;

    /*
    * Enquanto houver pacotes na pilha de pacotes do vizinho,
    * desempilha os pacotes e os empilha novamente no armazém.
    */
    while (!pacotes.Vazia()) {
        int pacote = pacotes.Desempilha();
        pilhaPacotes.Empilha(pacote);

        metricas.incStackPush();
        metricas.incReStorage();
        metricas.incPackagesMoved();

        std::cout << std::setfill('0')
                  << std::setw(7) << tempoAtual << " pacote "
                  << std::setw(3) << pacote << " rearmazenado em "
                  << std::setw(3) << this->_id - 1 << " na secao "
                  << std::setw(3) << idVizinho - 1 << std::endl;
    } 
}

/**
 * @brief Retorna a lista de transportes associados a um vizinho específico.
 *
 * @param idVizinho O identificador do vizinho cujos transportes serão recuperados.
 * @return Lista<int> Lista de transportes do vizinho especificado.
 * @throws std::runtime_error Se o vizinho não for encontrado.
 */
Lista<int> Armazem::getTransportesPorVizinho(int idVizinho) {
    Vizinho* vizinho = this->_dadosVizinho.getValor(idVizinho);
    if (vizinho == nullptr) {
        throw std::runtime_error("Vizinho não encontrado.");
    }
    return vizinho->transportes;
}


/**
 * @brief Obtém o valor de cooldown de um vizinho específico.
 *
 * @param idVizinho Identificador do vizinho cujo cooldown será recuperado.
 * @return int Valor do cooldown do vizinho.
 * @throws std::runtime_error Se o vizinho não for encontrado.
 */
int Armazem::getCooldown(int idVizinho) {
    Vizinho* vizinho = this->_dadosVizinho.getValor(idVizinho);

    if (vizinho == nullptr) {
        throw std::runtime_error("Vizinho não encontrado.");
    }

    return vizinho->cooldown;
}

/**
 * @brief Define o valor de cooldown para um vizinho específico.
 * 
 * @param idVizinho Identificador do vizinho cujo cooldown será definido.
 * @param cooldown Novo valor de cooldown a ser atribuído ao vizinho.
 * 
 * @throws std::runtime_error Se o vizinho com o id especificado não for encontrado.
 */
void Armazem::setCooldown(int idVizinho, int cooldown) {
    Vizinho* vizinho = this->_dadosVizinho.getValor(idVizinho);
    if (vizinho == nullptr) {
        throw std::runtime_error("Vizinho não encontrado.");
    }
    vizinho->cooldown = cooldown;
}

/**
 * @brief Retorna o identificador único do armazém.
 * 
 * @return int O ID do armazém.
 */
int Armazem::getId() const {
    return this->_id;
}

/**
 * @brief Define o identificador do armazém.
 * 
 * @param id Novo identificador a ser atribuído ao armazém.
 */
void Armazem::setId(int id) {
    this->_id = id;
}


/**
 * @brief Define a capacidade de um vizinho específico.
 *
 * @param idVizinho O identificador do vizinho cuja capacidade será alterada.
 * @param capacidade O novo valor de capacidade a ser atribuído ao vizinho.
 *
 * @throws std::runtime_error Se o vizinho com o ID especificado não for encontrado.
 */
void Armazem::setCapacidade(int idVizinho, int capacidade) {
    Vizinho* vizinho = this->_dadosVizinho.getValor(idVizinho);
    if (vizinho == nullptr) {
        throw std::runtime_error("Vizinho não encontrado.");
    }
    vizinho->capacidade = capacidade;
}

#endif