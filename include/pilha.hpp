#ifndef PILHA_HPP
#define PILHA_HPP

#include <stdexcept>
#include "node.hpp"

template <typename T>
class Pilha {
    private:
        Node<T>* _topo;
        int _tam;
        
    public:
        Pilha();
        Pilha(const Pilha<T>& other);
        Pilha<T>& operator=(const Pilha<T>& other);
        ~Pilha();

        void Empilha(T valor);
        T Desempilha();
        T Topo() const;
        bool Vazia() const;

        int GetTam() const;

};

/**
 * @brief Construtor padrão da classe Pilha.
 *
 * Inicializa uma nova pilha vazia, definindo o topo como nulo e o tamanho como zero.
 */
template <typename T>
Pilha<T>::Pilha() : _topo(nullptr), _tam(0) {}

/**
 * @brief Construtor de cópia da classe Pilha.
 *
 * Cria uma nova pilha copiando os elementos de outra pilha fornecida como parâmetro.
 * A nova pilha terá os mesmos elementos na mesma ordem da pilha original.
 *
 * @param other Referência constante para a pilha a ser copiada.
 */
template <typename T>
Pilha<T>::Pilha(const Pilha<T>& other) : _topo(nullptr) {
    if (other._topo == nullptr) {
        return; // Pilha vazia
    }
    Node<T>* current = other._topo;
    Node<T>* prev = nullptr;
    while (current != nullptr) {
        Node<T>* novo = new Node<T>(current->GetData());
        if (prev == nullptr) {
            this->_topo = novo; // Primeiro elemento
        } else {
            prev->SetNext(novo);
        }
        prev = novo;
        current = current->GetNext();
    }

    this->_tam = other._tam;
}

/**
 * @brief Operador de atribuição para a classe Pilha.
 *
 * Copia os elementos de outra pilha para a pilha atual, garantindo que não haja autoatribuição.
 * Limpa a pilha atual antes de copiar os elementos da outra pilha.
 *
 * @param other Referência constante para a pilha a ser copiada.
 * @return Referência para o próprio objeto Pilha após a atribuição.
 */
template <typename T>
Pilha<T>& Pilha<T>::operator=(const Pilha<T>& other) {
    if (this != &other) {
        // Limpa a pilha atual
        while (!this->Vazia()) {
            this->Desempilha();
        }
        // Copia os elementos da outra pilha
        Node<T>* current = other._topo;
        Node<T>* prev = nullptr;
        while (current != nullptr) {
            Node<T>* novo = new Node<T>(current->GetData());
            if (prev == nullptr) {
                this->_topo = novo; // Primeiro elemento
            } else {
                prev->SetNext(novo);
            }
            prev = novo;
            current = current->GetNext();
        }
    }
    this->_tam = other._tam;

    return *this;
}

/**
 * @brief Adiciona um novo elemento ao topo da pilha.
 *
 * @param valor O valor a ser adicionado à pilha.
 */
template <typename T>
void Pilha<T>::Empilha(T valor) {
    Node<T>* novo = new Node<T>(valor);
    novo->SetNext(this->_topo);

    this->_topo = novo;
    this->_tam++;
}

/**
 * @brief Remove e retorna o elemento do topo da pilha.
 *
 * @return O valor do elemento removido do topo da pilha.
 * @throws std::runtime_error Se a pilha estiver vazia.
 */
template <typename T>
T Pilha<T>::Desempilha() {
    if (Vazia()) {
        throw std::runtime_error("Pilha vazia");
    }

    Node<T>* temp = this->_topo;
    T valor = temp->GetData();
    // Atualiza o topo da pilha para o próximo nó
    this->_topo = this->_topo->GetNext();
    this->_tam--;

    delete temp;
    return valor;
}

/**
 * @brief Retorna o elemento do topo da pilha sem removê-lo.
 *
 * @return O valor do elemento no topo da pilha.
 * @throws std::runtime_error Se a pilha estiver vazia.
 */
template <typename T>
T Pilha<T>::Topo() const {
    if (Vazia()) {
        throw std::runtime_error("Pilha vazia");
    }
    return this->_topo->GetData();
}

/**
 * @brief Verifica se a pilha está vazia.
 *
 * @return true Se a pilha estiver vazia.
 * @return false Se a pilha contiver elementos.
 */
template <typename T>
bool Pilha<T>::Vazia() const {
    return this->_topo == nullptr;
}

/**
 * @brief Obtém o tamanho atual da pilha.
 *
 * @return O número de elementos na pilha.
 */
template <typename T>
int Pilha<T>::GetTam() const {
    return this->_tam;
}

/**
 * @brief Destrutor da classe Pilha.
 *
 * Libera a memória alocada para os nós da pilha, desempilhando todos os elementos.
 */
template <typename T>
Pilha<T>::~Pilha() {
    while (!this->Vazia()) {
        this->Desempilha();
    }
}

#endif