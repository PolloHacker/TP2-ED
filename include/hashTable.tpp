#include "hashTable.hpp"

template <typename T>
HashTable<T>::HashTable(int capacity) : 
    _table(capacity), _tam(0), _capacity(capacity) {}

template <typename T>
HashTable<T>::~HashTable() {
    // The Vetor and Lista destructors will handle memory cleanup.
}

template <typename T>
int HashTable<T>::hash(int key) {
    if (this->_capacity == 0) return 0;
    // Example: use a simple mixing step
    key = ((key >> 16) ^ key) * 0x45d9f3b;
    key = ((key >> 16) ^ key) * 0x45d9f3b;
    key = (key >> 16) ^ key;
    return std::abs(key * 31 % this->_capacity);
}

template <typename T>
int HashTable<T>::getTam() {
    return this->_tam;
}

template <typename T>
void HashTable<T>::insere(int key, T value) {
    int index = this->hash(key);
    Lista<KeyValue<T>>& bucket = this->_table[index];

    // Check if key already exists to update it
    for (int i = 1; i <= bucket.GetTam(); ++i) {
        Node<KeyValue<T>>* node = bucket.GetElemPos(i);
        if (node->GetDataRef().key == key) {
            node->GetDataRef().value = value; // Update existing value
            return;
        }
    }

    // If key does not exist, insert a new pair
    KeyValue<T> newPair;
    newPair.key = key;
    newPair.value = value;
    bucket.InsereFim(newPair);
    this->_tam++;
}

template <typename T>
void HashTable<T>::insereFim(int key, T value) {
    int index = this->hash(key);
    Lista<KeyValue<T>>& bucket = this->_table[index];

    KeyValue<T> newPair;
    newPair.key = key;
    newPair.value = value;
    bucket.InsereFim(newPair);
    this->_tam++;
}

template <typename T>
T* HashTable<T>::getValor(int key) {
    int index = this->hash(key);
    Lista<KeyValue<T>>& bucket = this->_table[index];

    if (bucket.GetTam() == 0) {
        return nullptr;
    }

    // Traverse the linked list (bucket) to find the key
    for (int i = 1; i <= bucket.GetTam(); ++i) {
        Node<KeyValue<T>>* node = bucket.GetElemPos(i);
        if (node->GetDataRef().key == key) {
            return &node->GetDataRef().value; // Return pointer to the value
        }
    }

    return nullptr; // Key not found
}

template <typename T>
bool HashTable<T>::removeChave(int key) {
    int index = this->hash(key);
    Lista<KeyValue<T>>& bucket = this->_table[index];

    // Traverse the bucket to find the key
    for (int i = 1; i <= bucket.GetTam(); ++i) {
        Node<KeyValue<T>>* node = bucket.GetElemPos(i);
        if (node->GetDataRef().key == key) {
            bucket.RemovePos(i);
            this->_tam--;
            return true;
        }
    }

    return false; // Key not found
}