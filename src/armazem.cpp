#include "armazem.hpp"
// #include "metricas.hpp"

Armazem::Armazem() : _id(-1) {}

Armazem::Armazem(int id) : _id(id) {}

Armazem::Armazem(const Armazem& other)
    : _id(other._id), 
      _vizinhos(other._vizinhos), 
      _pacotesPorVizinho(other._pacotesPorVizinho),
      _transportesPorVizinho(other._transportesPorVizinho),
      _cooldownsPorVizinho(other._cooldownsPorVizinho),
      _capacidadesPorVizinho(other._capacidadesPorVizinho) {}

Armazem& Armazem::operator=(const Armazem& other) {
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


void Armazem::adicionaVizinho(int vizinho) {
    this->_vizinhos.InsereFim(vizinho);

    this->_pacotesPorVizinho.insere(vizinho, Pilha<int>());
    this->_transportesPorVizinho.insere(vizinho, Lista<int>());
}

Lista<int> Armazem::getVizinhos() const {
    return this->_vizinhos;
}  

void Armazem::armazenaPacote(int idVizinho, int idPacote, Metricas& metricas) {
    Pilha<int>* pacoteStack = this->_pacotesPorVizinho.getValor(idVizinho);

    if (pacoteStack == nullptr) {
        throw std::runtime_error("Vizinho não encontrado.");
    }

    pacoteStack->Empilha(idPacote);

    metricas.incStackPush();
    metricas.sampleSectionDepth(pacoteStack->GetTam());
}

int Armazem::removePacotePorSecao(int idVizinho, int idPacote) {
    int* capacidadePtr = this->_capacidadesPorVizinho.getValor(idVizinho);
    if (capacidadePtr == nullptr) {
        throw std::runtime_error("Vizinho não encontrado.");
    }

    if (this->_pacotesPorVizinho.getValor(idVizinho)->Vazia()) {
        throw std::runtime_error("Nenhum pacote para enviar.");
    }

    Pilha<int> pilhaAux;
    Pilha<int>* pilhaPacotes = this->_pacotesPorVizinho.getValor(idVizinho);

    if (pilhaPacotes == nullptr) {
        throw std::runtime_error("Vizinho não encontrado.");
    }

    int curId = pilhaPacotes->Topo();

    while (curId != idPacote) {
        if (pilhaPacotes->Vazia()) {
            throw std::runtime_error("Pacote não encontrado.");
        }

        pilhaAux.Empilha(pilhaPacotes->Desempilha());
        if (pilhaPacotes->Vazia()) {
            throw std::runtime_error("Pacote não encontrado.");
        }
        curId = pilhaPacotes->Topo();
    }
    
    while (!pilhaAux.Vazia()) {
        pilhaPacotes->Empilha(pilhaAux.Desempilha());
    }

    return pilhaPacotes->Desempilha();
}

Pilha<int> Armazem::adicionaPacotesParaTransporte(int idVizinho, int& tempoAtual, int custoRemocao, Metricas& metricas) {
    int* capacidadePtr = this->_capacidadesPorVizinho.getValor(idVizinho);
    Lista<int>* transportePtr = this->_transportesPorVizinho.getValor(idVizinho);
    Pilha<int>* pilhaPacotesPtr = this->_pacotesPorVizinho.getValor(idVizinho);

    if (capacidadePtr == nullptr || transportePtr == nullptr || pilhaPacotesPtr == nullptr) {
        throw std::runtime_error("Vizinho não encontrado.");
    }

    int capacidade = *capacidadePtr;
    Lista<int>& transporte = *transportePtr;
    Pilha<int>& pilhaPacotes = *pilhaPacotesPtr;

    while (transporte.GetTam() > 0) {
        transporte.RemovePos(1);
    }

    Pilha<int> pilhaAux;
    int count = 0;

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

void Armazem::rearmazenarPacotes(int idVizinho, Pilha<int> pacotes, int tempoAtual, Metricas& metricas) {
    Pilha<int>* pilhaPacotesPtr = this->_pacotesPorVizinho.getValor(idVizinho);
    if (pilhaPacotesPtr == nullptr) {
        throw std::runtime_error("Vizinho não encontrado.");
    }

    Pilha<int>& pilhaPacotes = *pilhaPacotesPtr;

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

Lista<int> Armazem::getTransportesPorVizinho(int idVizinho) {
    return *this->_transportesPorVizinho.getValor(idVizinho);
}


int Armazem::getCooldown(int idVizinho) {
    int* coolPtr = this->_cooldownsPorVizinho.getValor(idVizinho);

    if (coolPtr == nullptr) {
        throw std::runtime_error("Vizinho não encontrado.");
    }

    return *coolPtr;
}

void Armazem::setCooldown(int idVizinho, int cooldown) {
    this->_cooldownsPorVizinho.insere(idVizinho, cooldown);
}

int Armazem::getId() const {
    return this->_id;
}

void Armazem::setId(int id) {
    this->_id = id;
}


void Armazem::setCapacidade(int idVizinho, int capacidade) {
    this->_capacidadesPorVizinho.insere(idVizinho, capacidade);
}