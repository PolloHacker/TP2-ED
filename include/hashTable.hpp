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

        int hash(int key);

    public:
        HashTable(int capacity = 7); // Default to a small prime number

        // Inserts a key-value pair. If the key exists, it updates the value.
        void insere(int key, T value);
        void insereFim(int key, T value);

        // Retrieves a pointer to the value for a given key.
        // Returns nullptr if the key is not found.
        T* getValor(int key);

        // Removes a key-value pair. Returns true if successful, false otherwise.
        bool removeChave(int key);

        // Returns the number of items stored in the hash table.
        int getTam();

};

#include "hashTable.tpp"

#endif