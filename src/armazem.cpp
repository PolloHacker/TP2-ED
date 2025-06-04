#include "armazem.hpp"

Armazem::Armazem(int id) : _id(id) {}

int Armazem::buscaVizinho(int id) const {
    Node<Armazem>* aux = _vizinhos._head->GetNext();
    int pos = 1;

    while (aux != nullptr) {
        if (aux->GetData().getId() == id) {
            return pos;
        }
        aux = aux->GetNext();
        pos++;
    }
    return -1;
}

void Armazem::adicionaVizinho(Armazem& vizinho) {
    Node<Armazem> aux = Node<Armazem>(vizinho);
    this->_vizinhos.InsereFim(aux);
}

void Armazem::removeVizinho(int id) {
    int pos = this->buscaVizinho(id);
    this->_vizinhos.RemovePos(pos);
}

Lista<Armazem> Armazem::getVizinhos() const {
    return this->_vizinhos;
}  


void Armazem::recebePacote(const Pacote<std::string>& pacote) {
    this->_pacotes.Empilha(pacote);
}

Pacote<std::string> Armazem::enviaPacote() {
    if (this->_pacotes.Vazia()) {
        throw std::runtime_error("Nenhum pacote para enviar.");
    }
    return this->_pacotes.Desempilha();
}

Pilha<Pacote<std::string>> Armazem::getPacotes() const {
    return this->_pacotes;
}

int Armazem::getId() const {
    return this->_id;
}

void Armazem::setId(int id) {
    this->_id = id;
}
