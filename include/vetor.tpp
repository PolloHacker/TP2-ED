#pragma once

#include "vetor.hpp"

/**
 * @brief Construtor da classe Vetor.
 * 
 * Aloca dinamicamente um array de elementos do tipo T com o tamanho especificado.
 * 
 * @param size O tamanho do vetor a ser criado.
 */
template <typename T>
Vetor<T>::Vetor(int size): _size(size), _capacity(size) {
    if (size <= 0) {
        throw std::invalid_argument("Tamanho do vetor deve ser maior que zero.");
    }
    this->_data = new T[size];
    printf("Vetor criado com tamanho %d e capacidade %d.\n", size, size);
}

/**
 * @brief Construtor de cópia da classe Vetor.
 *
 * Cria uma nova instância de Vetor copiando os elementos do vetor passado como parâmetro.
 * Aloca um novo bloco de memória para armazenar os dados, garantindo que as alterações em um vetor
 * não afetem o outro.
 *
 * @param other Referência constante para o Vetor a ser copiado.
 */
template <typename T>
Vetor<T>::Vetor(const Vetor& other) {
    this->_data = new T[other._capacity];
    for (int i = 0; i < other._size; i++) {
        this->_data[i] = other._data[i];
    }
    this->_size = other._size;
    this->_capacity = other._capacity;
}

/**
 * @brief Redimensiona o vetor para dobrar sua capacidade.
 * 
 * Cria um novo array com o dobro da capacidade atual, copia os elementos do vetor original
 * para o novo array e libera a memória do vetor original.
 */
template <typename T>
void Vetor<T>::resize(int newCapacity) {
    if (newCapacity <= 0 || newCapacity < this->_size) {
        throw std::invalid_argument("Nova capacidade deve ser maior do que o tamanho atual.");
    }
    T* newData = new T[newCapacity];

    for (int i = 0; i < std::min(_size, newCapacity); i++) {
        newData[i] = _data[i];
    }

    delete[] _data;
    this->_data = newData;
    this->_capacity = newCapacity;
}

/**
 * @brief Verifica se o vetor precisa ser redimensionado.
 * 
 * Se o tamanho atual do vetor for igual à sua capacidade, aumenta a capacidade do vetor.
 */
template <typename T>
void Vetor<T>::shouldResize(int indx) {
    printf("Verificando se o vetor precisa ser redimensionado para o índice %d.\n", indx);
    if (indx + 1 >= this->_capacity) {
        this->resize(this->_capacity * 2);
        printf("Vetor redimensionado para capacidade %d.\n", this->_capacity);
        printf("Novo tamanho do vetor: %d.\n", this->_size);
    }
}


template <typename T>
/**
 * @brief Insere um elemento no vetor na posição especificada.
 * 
 * Se o índice estiver fora dos limites do vetor, lança uma exceção.
 * Se o vetor estiver cheio, redimensiona-o antes de inserir o novo elemento.
 * 
 * @param index Índice onde o elemento será inserido.
 * @param value Valor do elemento a ser inserido.
 */
void Vetor<T>::insere(int index, const T& value) {
    if (index < 0) {
        throw std::out_of_range("Índice fora dos limites do vetor.");
    }

    this->shouldResize(index);
    this->_data[index] = value;
    if (index >= this->_size) {
        this->_size = index + 1;
    }
}

/**
 * @brief Remove o elemento na posição especificada do vetor.
 * 
 * Se o índice estiver fora dos limites do vetor, lança uma exceção.
 * 
 * @param index Índice do elemento a ser removido.
 */
template <typename T>
void Vetor<T>::remove(int index) {
    if (index < 0 || index >= this->_size) {
        throw std::out_of_range("Índice fora dos limites do vetor.");
    }

    for (int i = index; i < this->_size - 1; i++) {
        this->_data[i] = this->_data[i + 1];
    }
    this->_size--;
}

/**
 * @brief Sobrecarga do operador de indexação para acessar elementos do vetor.
 * 
 * Permite acessar e modificar o elemento na posição especificada pelo índice.
 * Não realiza verificação de limites; o comportamento é indefinido se o índice estiver fora do intervalo válido.
 * 
 * @param index Índice do elemento a ser acessado.
 * @return Referência ao elemento do tipo T na posição especificada.
 */
template <typename T>
T& Vetor<T>::operator[](int index) {
    return this->_data[index];
}

/**
 * @brief Operador de acesso constante ao elemento do vetor.
 * 
 * Permite acessar um elemento do vetor pelo índice fornecido, sem permitir modificação.
 * 
 * @param index Índice do elemento a ser acessado.
 * @return Referência constante para o elemento na posição especificada.
 */
template <typename T>
const T& Vetor<T>::operator[](int index) const {
    return this->_data[index];
}

/**
 * @brief Sobrecarga do operador de atribuição para copiar o conteúdo de outro Vetor.
 *
 * Realiza uma cópia profunda dos dados do vetor fornecido, garantindo que cada instância
 * tenha sua própria alocação de memória.
 *
 * @param other Referência constante para o Vetor a ser copiado.
 * @return Referência ao Vetor atual após a atribuição.
 */
template <typename T>
Vetor<T>& Vetor<T>::operator=(const Vetor<T>& other) {
    if (this == &other) return *this;

    delete[] this->_data;
    this->_data = new T[other._capacity];

    for (int i = 0; i < other._size; i++) {
        this->_data[i] = other._data[i];
    }
    this->_size = other._size;
    this->_capacity = other._capacity;

    return *this;
}

/**
 * @brief Destrutor da classe Vetor.
 *
 * Libera a memória alocada dinamicamente para o array interno (_data).
 * Garante que não haja vazamento de memória ao destruir um objeto Vetor.
 */
template <typename T>
Vetor<T>::~Vetor() {
    delete[] this->_data;
}