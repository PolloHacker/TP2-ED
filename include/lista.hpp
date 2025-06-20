#ifndef LISTA_HPP
#define LISTA_HPP

#include <iostream>
#include <exception>
#include <string>
#include "node.hpp"

template <typename T>
class Lista {
    private:
        int _tam;
        
    public:
        Lista();
        Lista(const Lista<T>& other);
        Lista<T>& operator=(const Lista<T>& other);
        ~Lista();

        Node<T>* Posiciona(int pos);
        void InsereInicio(T& obj);
        void InserePos(int pos, T& obj);
        void InsereFim(T& obj);
        
        T RemovePos(int pos);
        Node<T>* GetElemPos(int pos);

        int GetTam();
        Node<T>* _head;
        Node<T>* _tail;
        

};


/**
 * @brief Construtor padrão da classe Lista.
 *
 * Inicializa uma nova lista vazia, criando um nó auxiliar que serve como cabeça e cauda da lista.
 * O tamanho da lista (_tam) é definido como zero.
 */
template <typename T>
Lista<T>::Lista() : _tam(0) {
    Node<T>* aux = new Node<T>();
    this->_head = aux;
    this->_tail = aux;
}

/**
 * @brief Construtor de cópia da classe Lista.
 *
 * Cria uma nova lista copiando os elementos de outra lista fornecida como parâmetro.
 * O tamanho da nova lista é definido com base no tamanho da lista original.
 *
 * @param other Referência constante para a lista a ser copiada.
 */
template <typename T>
Lista<T>::Lista(const Lista<T>& other) : _tam(other._tam) {
    this->_head = new Node<T>();
    Node<T>* current = this->_head;

    Node<T>* otherCurrent = other._head->GetNext();
    while (otherCurrent != nullptr) {
        Node<T>* newNode = new Node<T>(otherCurrent->GetData());
        current->SetNext(newNode);
        current = newNode;
        otherCurrent = otherCurrent->GetNext();
    }
    this->_tail = current;
}

/**
 * @brief Operador de atribuição para a classe Lista.
 *
 * Copia os elementos de outra lista para a lista atual, garantindo que não haja autoatribuição.
 * Limpa a lista atual antes de copiar os elementos da outra lista.
 *
 * @param other Referência constante para a lista a ser copiada.
 * @return Referência para o próprio objeto Lista após a atribuição.
 */
template <typename T>
Lista<T>& Lista<T>::operator=(const Lista<T>& other) {
    if (this != &other) {
        // Limpa a lista atual
        Node<T>* current = this->_head;
        while (current != nullptr) {
            Node<T>* next = current->GetNext();
            delete current;
            current = next;
        }
        
        // Copia os dados da outra lista
        this->_tam = other._tam;
        this->_head = new Node<T>();
        Node<T>* newCurrent = this->_head;

        Node<T>* otherCurrent = other._head->GetNext();
        while (otherCurrent != nullptr) {
            Node<T>* newNode = new Node<T>(otherCurrent->GetData());
            newCurrent->SetNext(newNode);
            newCurrent = newNode;
            otherCurrent = otherCurrent->GetNext();
        }
        this->_tail = newCurrent;
    }
    return *this;
}

/**
 * @brief Posiciona o ponteiro na posição especificada da lista.
 *
 * Este método retorna o nó na posição 'pos' da lista, onde a posição é baseada em 1.
 *
 * @param pos Posição do nó a ser retornado (baseada em 1).
 * @return Node<T>* Ponteiro para o nó na posição especificada.
 * @throws std::invalid_argument Se a posição for inválida.
 */
template <typename T>
Node<T>* Lista<T>::Posiciona(int pos) {
    int i;
    Node<T>* aux;

    // Verifica se a posição é válida
    if (this->_tam < pos || pos < 0) {
        throw std::invalid_argument("Posicao invalida");
    }

    aux = this->_head;

    // Percorre a lista até a posição especificada
    // O nó na posição 0 é o nó cabeça, então começamos do próximo nó
    for (i = 1; i <= pos ; i++) {
        aux = aux->GetNext();
    }

    return aux;
}

/**
 * @brief Insere um novo elemento no início da lista.
 *
 * @param obj Referência ao objeto a ser inserido no início da lista.
 */
template <typename T>
void Lista<T>::InsereInicio(T& obj) {
    // Cria um novo nó com o objeto fornecido e o insere no início da lista
    Node<T>* entry = new Node<T>(obj);
    entry->SetNext(this->_head->GetNext());

    this->_head->SetNext(entry);
    this->_tam++;

    // Atualiza a cauda da lista se for a primeira inserção
    if (this->_tail == nullptr) {
        this->_tail = entry;
    }
}

/**
 * @brief Insere um novo elemento na posição especificada da lista.
 *
 * @param pos Posição onde o novo elemento será inserido (baseada em 1).
 * @param obj Referência ao objeto a ser inserido.
 * @throws std::invalid_argument Se a posição for inválida.
 */
template <typename T>
void Lista<T>::InserePos(int pos, T& obj) {
    // Pega a posição do nó antes da posição onde o novo elemento será inserido
    Node<T>* before = this->Posiciona(pos - 1);
    Node<T>* entry = new Node<T>(obj);

    // Insere o novo nó na posição especificada, ou seja, entre o nó 'before' e o próximo nó
    entry->SetNext(before->GetNext());
    before->SetNext(entry);
    this->_tam++;
    
    // Se o novo nó for o último, atualiza a cauda da lista
    if (entry->GetNext() == nullptr) {
        this->_tail = entry;
    }
}

/**
 * @brief Insere um novo elemento no final da lista.
 *
 * @param obj Referência ao objeto a ser inserido no final da lista.
 */
template <typename T>
void Lista<T>::InsereFim(T& obj) {
    Node<T>* entry = new Node<T>(obj);
    this->_tail->SetNext(entry);
    this->_tail = entry;
    this->_tam++;
}

/**
 * @brief Remove o elemento na posição especificada da lista.
 *
 * @param pos Posição do nó a ser removido (baseada em 1).
 * @return T O dado do nó removido.
 * @throws std::invalid_argument Se a lista estiver vazia.
 */
template <typename T>
T Lista<T>::RemovePos(int pos) {
    if (this->_tam == 0) {
        throw std::invalid_argument("Lista vazia");
    }

    // Pega o nó antes da posição onde o elemento será removido
    Node<T>* before = this->Posiciona(pos - 1);
    Node<T>* to_del = before->GetNext();

    // Atualiza o ponteiro do nó anterior para pular o nó a ser removido
    before->SetNext(to_del->GetNext());
    this->_tam--;

    // Se o nó removido for a cauda, atualiza a cauda da lista
    if (before->GetNext() == nullptr) {
        this->_tail = before;
    }

    T data = to_del->GetData();
    delete to_del;

    return data;
}


/**
 * @brief Obtém o elemento na posição especificada da lista.
 *
 * @param pos Posição do nó a ser retornado (baseada em 1).
 * @return Node<T>* Ponteiro para o nó na posição especificada.
 * @throws std::invalid_argument Se a posição for inválida.
 */
template <typename T>
Node<T>* Lista<T>::GetElemPos(int pos) {
    Node<T>* aux;

    aux = this->Posiciona(pos);
    return aux;
}


/**
 * @brief Obtém o tamanho atual da lista.
 *
 * @return int O número de elementos na lista.
 */
template <typename T>
int Lista<T>::GetTam() {
    return this->_tam;
}


/**
 * @brief Destrutor da classe Lista.
 *
 * Libera a memória alocada para todos os nós da lista, garantindo que não haja vazamentos de memória.
 */
template <typename T>
Lista<T>::~Lista() {
    Node<T>* curr = this->_head;

    while (curr != nullptr) {
        Node<T>* next = curr->GetNext();
        delete curr;
        curr = next;
    }
    this->_head = nullptr;
    this->_tail = nullptr;
}

#endif