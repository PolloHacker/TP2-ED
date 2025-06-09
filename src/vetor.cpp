#include "vetor.hpp"

/**
 * @brief Construtor da classe Vetor.
 * 
 * Aloca dinamicamente um array de elementos do tipo T com o tamanho especificado.
 * 
 * @param size O tamanho do vetor a ser criado.
 */
template <typename T>
Vetor<T>::Vetor(int size): _size(size) {
    this->_data = new T[size];
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
    this->_data = new T[other._size];
    for (int i = 0; i < other._size; i++) {
        this->_data[i] = other._data[i];
    }
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
 * @brief Destrutor da classe Vetor.
 *
 * Libera a memória alocada dinamicamente para o array interno (_data).
 * Garante que não haja vazamento de memória ao destruir um objeto Vetor.
 */
template <typename T>
Vetor<T>::~Vetor() {
    delete[] this->_data;
}

template class Vetor<int>;