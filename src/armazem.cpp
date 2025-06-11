#include "armazem.hpp"

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
    Pilha<int> pilhaAux;
    Lista<int> listaAux;

    this->_vizinhos.InsereFim(vizinho);
    this->_pacotesPorVizinho.InsereFim(pilhaAux);
    this->_transportesPorVizinho.InsereFim(listaAux);
}

void Armazem::removeVizinho(int id) {
    int pos = this->buscaVizinho(id);
    this->_vizinhos.RemovePos(pos);
    this->_pacotesPorVizinho.RemovePos(pos);
}

Lista<int> Armazem::getVizinhos() const {
    return this->_vizinhos;
}  

void Armazem::armazenaPacote(int idVizinho, int idPacote) {
    int pos = this->buscaVizinho(idVizinho);
    if (pos == -1) {
        throw std::runtime_error("Vizinho não encontrado.");
    }

    this->_pacotesPorVizinho.Posiciona(pos)->GetData().Empilha(idPacote);
}

int Armazem::removePacotePorSecao(int idVizinho, int idPacote) {
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

void Armazem::adicionaPacoteParaTransporte(int idVizinho, int IdPacote) {
    int pos = this->buscaVizinho(idVizinho);
    if (pos == -1) {
        throw std::runtime_error("Vizinho não encontrado.");
    }

    if (this->_transportesPorVizinho.Posiciona(pos)->GetData().GetTam() >= this->_capacidadesPorVizinho.Posiciona(pos)->GetData()) {
        throw std::runtime_error("Transporte já está cheio para este vizinho.");
    }

    this->_transportesPorVizinho.Posiciona(pos)->GetDataRef().InsereFim(IdPacote);
    std::cout << "Pacote " << IdPacote << " adicionado para transporte no vizinho " << idVizinho << std::endl;
    std::cout << "Pacotes no transporte do vizinho " << idVizinho << ": ";
    auto lista = this->_transportesPorVizinho.Posiciona(pos)->GetData();
    auto node = lista._head->GetNext();
    while (node != nullptr) {
        std::cout << node->GetData() << " ";
        node = node->GetNext();
    }
    std::cout << std::endl;
}

Lista<int> Armazem::getTransportesPorVizinho(int idVizinho) {
    int pos = this->buscaVizinho(idVizinho);
    if (pos == -1) {
        throw std::runtime_error("Vizinho não encontrado.");
    }

    return this->_transportesPorVizinho.Posiciona(pos)->GetData();
}

bool Armazem::verificaSecoesVazias() {
    for (int i = 1; i <= this->_vizinhos.GetTam(); ++i) {
        if (!this->_pacotesPorVizinho.Posiciona(i)->GetData().Vazia()) {
            return false;
        }
    }
    return true;
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
    if (this->_cooldownsPorVizinho.GetTam() < pos + 1) {
        this->_cooldownsPorVizinho.InsereFim(cooldown);
        return;
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

    if (this->_capacidadesPorVizinho.GetTam() < pos + 1) {
        this->_capacidadesPorVizinho.InsereFim(capacidade);
        return;
    }
    this->_capacidadesPorVizinho.Posiciona(pos)->SetData(capacidade);
}