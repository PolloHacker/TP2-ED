#include "armazem.hpp"

Armazem::Armazem() : _id(-1) {}

Armazem::Armazem(int id) : _id(id) {}

// Copy constructor
Armazem::Armazem(const Armazem& other) 
    : _id(other._id),
      _vizinhos(other._vizinhos),
      _pacotesPorVizinho(other._pacotesPorVizinho),
      _transportesPorVizinho(other._transportesPorVizinho),
      _cooldownsPorVizinho(other._cooldownsPorVizinho),
      _capacidadesPorVizinho(other._capacidadesPorVizinho) {}

// Assignment operator
Armazem& Armazem::operator=(const Armazem& other) {
    if (this != &other) {
        _id = other._id;
        _vizinhos = other._vizinhos;
        _pacotesPorVizinho = other._pacotesPorVizinho;
        _transportesPorVizinho = other._transportesPorVizinho;
        _cooldownsPorVizinho = other._cooldownsPorVizinho;
        _capacidadesPorVizinho = other._capacidadesPorVizinho;
    }
    return *this;
}

int Armazem::buscaVizinho(int id) const {
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

void Armazem::adicionaVizinho(int vizinho) {
    this->_vizinhos.InsereFim(vizinho);
    Pilha<Pacote<std::string>> pilhaAux;
    this->_pacotesPorVizinho.InsereFim(pilhaAux);
}

void Armazem::removeVizinho(int id) {
    int pos = this->buscaVizinho(id);
    this->_vizinhos.RemovePos(pos);
    this->_pacotesPorVizinho.RemovePos(pos);
}

Lista<int> Armazem::getVizinhos() const {
    return this->_vizinhos;
}  

void Armazem::armazenaPacote(const Pacote<std::string>& pacote, int idVizinho) {
    int pos = this->buscaVizinho(idVizinho);
    if (pos == -1) {
        throw std::runtime_error("Vizinho não encontrado.");
    }

    this->_pacotesPorVizinho.Posiciona(pos)->GetData().Empilha(pacote);
}

Pacote<std::string> Armazem::removePacotePorSecao(int idVizinho, int idPacote) {
    int pos = this->buscaVizinho(idVizinho);
    if (pos == -1) {
        throw std::runtime_error("Vizinho não encontrado.");
    }

    if (this->_pacotesPorVizinho.Posiciona(pos)->GetData().Vazia()) {
        throw std::runtime_error("Nenhum pacote para enviar.");
    }

    Pilha<Pacote<std::string>> pilhaAux;
    int curId = this->_pacotesPorVizinho.Posiciona(pos)->GetData().Topo().getId();

    while (curId != idPacote) {
        if (this->_pacotesPorVizinho.Posiciona(pos)->GetData().Vazia()) {
            throw std::runtime_error("Pacote não encontrado.");
        }

        pilhaAux.Empilha(
            this->_pacotesPorVizinho.Posiciona(pos)->GetData().Desempilha());
        curId = this->_pacotesPorVizinho.Posiciona(pos)->GetData().Topo().getId();
    }
    
    while (!pilhaAux.Vazia()) {
        this->_pacotesPorVizinho
            .Posiciona(pos)->GetData().Empilha(pilhaAux.Desempilha());
    }

    return this->_pacotesPorVizinho.Posiciona(pos)->GetData().Desempilha();
}

void Armazem::adicionaPacoteParaTransporte(int idVizinho, const Pacote<std::string>& pacote) {
    int pos = this->buscaVizinho(idVizinho);
    if (pos == -1) {
        throw std::runtime_error("Vizinho não encontrado.");
    }

    if (this->_transportesPorVizinho.Posiciona(pos)->GetData().GetTam() >= this->_capacidadesPorVizinho.Posiciona(pos)->GetData()) {
        throw std::runtime_error("Transporte já está cheio para este vizinho.");
    }

    int idToInsert = pacote.getId();
    this->_transportesPorVizinho.Posiciona(pos)->GetData().InsereFim(idToInsert);
}

Lista<int> Armazem::getTransportesPorVizinho(int idVizinho) {
    int pos = this->buscaVizinho(idVizinho);
    if (pos == -1) {
        throw std::runtime_error("Vizinho não encontrado.");
    }

    return this->_transportesPorVizinho.Posiciona(pos)->GetData();
}

int Armazem::getCooldown(int idVizinho) {
    int pos = this->buscaVizinho(idVizinho);
    if (pos == -1) {
        throw std::runtime_error("Vizinho não encontrado.");
    }

    return this->_cooldownsPorVizinho.Posiciona(pos)->GetData();
}

void Armazem::setCooldown(int idVizinho, int cooldown) {
    int pos = this->buscaVizinho(idVizinho);
    if (pos == -1) {
        throw std::runtime_error("Vizinho não encontrado.");
    }

    this->_cooldownsPorVizinho.Posiciona(pos)->SetData(cooldown);
}

int Armazem::getId() const {
    return this->_id;
}

void Armazem::setId(int id) {
    this->_id = id;
}


void Armazem::setCapacidade(int idVizinho, int capacidade) {
    int pos = this->buscaVizinho(idVizinho);
    if (pos == -1) {
        throw std::runtime_error("Vizinho não encontrado.");
    }

    this->_capacidadesPorVizinho.Posiciona(pos)->SetData(capacidade);
}