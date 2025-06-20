#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include "vetor.hpp"
#include "lista.hpp"

template <typename T>
struct KeyValue {
    int key;
    T value;
};

template <typename T>
class HashTable {
    private:
        Vetor<Lista<KeyValue<T>>> _table;
        int _tam;
        int _capacity;

        int hash(int key);    public:
        HashTable(int capacity = 7);

        void insere(int key, T value);
        void insereFim(int key, T value);

        T* getValor(int key);
        bool removeChave(int key);


        int getTam();

};


/**
 * @brief Construtor da classe HashTable.
 * 
 * Inicializa uma nova tabela hash com a capacidade especificada.
 * 
 * @param capacity Capacidade inicial da tabela hash.
 */
template <typename T>
HashTable<T>::HashTable(int capacity) : 
    _table(capacity), _tam(0), _capacity(capacity) {}


/**
 * @brief Calcula o valor do hash para uma chave inteira.
 *
 * Esta função aplica uma série de operações de mistura (mixing) na chave fornecida
 * para gerar um valor de hash distribuído uniformemente. O resultado é ajustado
 * para garantir que esteja dentro dos limites da capacidade da tabela hash.
 *
 * @param key Chave inteira a ser processada.
 * @return int Índice calculado para a chave, no intervalo [0, _capacity).
 */
template <typename T>
int HashTable<T>::hash(int key) {
    if (this->_capacity == 0) return 0;

    key = ((key >> 16) ^ key) * 0x45d9f3b;
    key = ((key >> 16) ^ key) * 0x45d9f3b;
    key = (key >> 16) ^ key;

    return std::abs(key * 31 % this->_capacity);
}

/**
 * @brief Obtém o tamanho atual da tabela hash.
 *
 * Esta função retorna o número de elementos atualmente armazenados na tabela hash.
 *
 * @return int Tamanho atual da tabela hash.
 */
template <typename T>
int HashTable<T>::getTam() {
    return this->_tam;
}

/**
 * @brief Insere um par chave-valor na tabela hash.
 *
 * @param key Chave inteira a ser inserida.
 * @param value Valor associado à chave a ser inserido.
 */
template <typename T>
void HashTable<T>::insere(int key, T value) {
    // Calcula o índice do bucket usando a função hash
    int index = this->hash(key);
    Lista<KeyValue<T>>& bucket = this->_table[index];

    // Se a chave já existir, atualiza o valor
    for (int i = 1; i <= bucket.GetTam(); ++i) {
        Node<KeyValue<T>>* node = bucket.GetElemPos(i);
        if (node->GetDataRef().key == key) {
            node->GetDataRef().value = value;
            return;
        }
    }

    // Se a chave não existir, cria um novo par e insere no bucket
    KeyValue<T> newPair;
    newPair.key = key;
    newPair.value = value;
    bucket.InsereFim(newPair);
    this->_tam++;
}

/**
 * @brief Insere um par chave-valor no final da lista do bucket correspondente.
 *
 * @param key Chave inteira a ser inserida.
 * @param value Valor associado à chave a ser inserido.
 */
template <typename T>
void HashTable<T>::insereFim(int key, T value) {
    // Calcula o índice do bucket usando a função hash
    int index = this->hash(key);
    Lista<KeyValue<T>>& bucket = this->_table[index];

    // Se a chave já existir, atualiza o valor
    KeyValue<T> newPair;
    newPair.key = key;
    newPair.value = value;
    bucket.InsereFim(newPair);
    this->_tam++;
}

/**
 * @brief Obtém o valor associado a uma chave específica.
 *
 * @param key Chave inteira cujo valor será recuperado.
 * @return T* Ponteiro para o valor associado à chave, ou nullptr se a chave não existir.
 */
template <typename T>
T* HashTable<T>::getValor(int key) {
    // Calcula o índice do bucket usando a função hash
    int index = this->hash(key);
    Lista<KeyValue<T>>& bucket = this->_table[index];

    if (bucket.GetTam() == 0) {
        return nullptr;
    }

    // Anda pela lista do bucket para encontrar a chave
    for (int i = 1; i <= bucket.GetTam(); ++i) {
        Node<KeyValue<T>>* node = bucket.GetElemPos(i);
        if (node->GetDataRef().key == key) {
            return &node->GetDataRef().value;
        }
    }

    // Se a chave não for encontrada, retorna nullptr
    return nullptr;
}

/**
 * @brief Remove um par chave-valor da tabela hash com base na chave.
 *
 * @param key Chave inteira do par a ser removido.
 * @return bool Retorna true se a chave foi removida com sucesso, ou false se a chave não foi encontrada.
 */
template <typename T>
bool HashTable<T>::removeChave(int key) {
    int index = this->hash(key);
    Lista<KeyValue<T>>& bucket = this->_table[index];

    // Anda pela lista do bucket para encontrar a chave
    for (int i = 1; i <= bucket.GetTam(); ++i) {
        Node<KeyValue<T>>* node = bucket.GetElemPos(i);
        
        // Se a chave for encontrada, remove o nó
        if (node->GetDataRef().key == key) {
            bucket.RemovePos(i);
            this->_tam--;
            return true;
        }
    }

    // Se a chave não for encontrada, retorna false
    return false;
}

#endif