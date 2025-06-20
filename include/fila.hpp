#ifndef FILA_HPP
#define FILA_HPP

#include <stdexcept>
#include "node.hpp"

template <typename T>
class Fila {
    private:
        Node<T>* _inicio;
        Node<T>* _fim;
        
    public:
        Fila();
        Fila(const Fila<T>& other);
        Fila<T>& operator=(const Fila<T>& other);
        ~Fila();

        void Enfileira(T valor);
        T Desenfileira();
        T Frente() const;
        bool Vazia() const;

};

/**
 * @brief Construtor padrão da classe Fila.
 *
 * Inicializa uma nova fila vazia, definindo os ponteiros de início e fim como nulos.
 */
template <typename T>
Fila<T>::Fila() : _inicio(nullptr), _fim(nullptr) {}

/**
 * @brief Construtor de cópia da classe Fila.
 *
 * Cria uma nova fila copiando os elementos de outra fila fornecida como parâmetro.
 *
 * @param other Referência constante para a fila a ser copiada.
 */
template <typename T>
Fila<T>::Fila(const Fila<T>& other) : _inicio(nullptr), _fim(nullptr) {
    Node<T>* current = other._inicio;
    while (current != nullptr) {
        this->Enfileira(current->GetData());
        current = current->GetNext();
    }
}

/**
 * @brief Operador de atribuição para a classe Fila.
 *
 * Copia os elementos de outra fila para a fila atual, garantindo que não haja autoatribuição.
 *
 * @param other Referência constante para a fila a ser copiada.
 * @return Referência para o próprio objeto Fila após a atribuição.
 */
template <typename T>
Fila<T>& Fila<T>::operator=(const Fila<T>& other) {
    if (this != &other) {
        // Limpa a fila atual
        while (!this->Vazia()) {
            this->Desenfileira();
        }
        // Copia os elementos da outra fila
        Node<T>* current = other._inicio;
        while (current != nullptr) {
            this->Enfileira(current->GetData());
            current = current->GetNext();
        }
    }
    return *this;
}

/**
 * @brief Adiciona um novo elemento ao final da fila.
 *
 * @param valor O valor a ser adicionado à fila.
 */
template <typename T>
void Fila<T>::Enfileira(T valor) {
    Node<T>* novo = new Node<T>(valor);

    // Se a fila estiver vazia, o novo nó se torna o início e o fim da fila.
    // Caso contrário, adiciona o novo nó ao final da fila.
    if (Vazia()) {
        this->_inicio = novo;
        this->_fim = novo;
    } else {
        this->_fim->SetNext(novo);
        this->_fim = novo;
    }
}

/**
 * @brief Remove e retorna o elemento do início da fila.
 *
 * @return O valor do elemento removido da fila.
 * @throws std::runtime_error Se a fila estiver vazia.
 */
template <typename T>
T Fila<T>::Desenfileira() {
    // Verifica se a fila está vazia antes de tentar remover um elemento.
    if (Vazia()) {
        throw std::runtime_error("Fila vazia");
    }
    // Remove o nó do início da fila e atualiza o ponteiro de início.
    Node<T>* temp = this->_inicio;
    T valor = temp->GetData();
    this->_inicio = this->_inicio->GetNext();

    // Se o início da fila se torna nulo, também atualiza o ponteiro de fim.
    if (this->_inicio == nullptr) {
        this->_fim = nullptr;
    }

    delete temp;
    return valor;
}

/**
 * @brief Retorna o valor do elemento no início da fila sem removê-lo.
 *
 * @return O valor do elemento no início da fila.
 * @throws std::runtime_error Se a fila estiver vazia.
 */
template <typename T>
T Fila<T>::Frente() const {
    if (this->Vazia()) {
        throw std::runtime_error("Fila vazia");
    }
    return this->_inicio->GetData();
}

/**
 * @brief Verifica se a fila está vazia.
 *
 * @return true se a fila estiver vazia, false caso contrário.
 */
template <typename T>
bool Fila<T>::Vazia() const {
    return this->_inicio == nullptr;
}

/**
 * @brief Destrutor da classe Fila.
 *
 * Libera a memória alocada para os nós da fila, removendo todos os elementos.
 */
template <typename T>
Fila<T>::~Fila() {
    while (!this->Vazia()) {
        this->Desenfileira();
    }
}

#endif