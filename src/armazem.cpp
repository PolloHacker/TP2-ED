#include "armazem.hpp"

Armazem::Armazem() : _id(-1) {}

Armazem::Armazem(int id) : _id(id) {
    std::cout << "Armazém criado com ID: " << this->_id << std::endl;
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
    std::cout << "Vizinho " << vizinho << " adicionado ao armazém " << this->_id << "." << std::endl;
}

void Armazem::removeVizinho(int id) {
    int pos = this->buscaVizinho(id);
    this->_vizinhos.RemovePos(pos);
    this->_pacotesPorVizinho.RemovePos(pos);
    std::cout << "Vizinho " << id << " removido do armazém " << this->_id << "." << std::endl;
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

int Armazem::getId() const {
    return this->_id;
}

void Armazem::setId(int id) {
    this->_id = id;
}
