#ifndef VETOR_HPP
#define VETOR_HPP

#include <stdexcept>

template <typename T>
class Vetor {
    private:
        T* _data;
        int _size;
        int _capacity;
        void shouldResize(int indx);
        void resize(int newCapacity);
    public:
        Vetor(int size);
        Vetor(const Vetor& other);
        ~Vetor();
        int getSize() const;
        void insere(int index, const T& value);
        void remove(int index);
        T& operator[](int index);
        const T& operator[](int index) const;
        Vetor<T>& operator=(const Vetor<T>& other);
};

// Implementation

template <typename T>
Vetor<T>::Vetor(int size): _size(size), _capacity(size) {
    if (size <= 0) {
        throw std::invalid_argument("Tamanho do vetor deve ser maior que zero.");
    }
    this->_data = new T[size];
}

template <typename T>
Vetor<T>::Vetor(const Vetor& other) {
    this->_data = new T[other._capacity];
    for (int i = 0; i < other._size; i++) {
        this->_data[i] = other._data[i];
    }
    this->_size = other._size;
    this->_capacity = other._capacity;
}

template <typename T>
int Vetor<T>::getSize() const {
    return this->_size;
}

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

template <typename T>
void Vetor<T>::shouldResize(int indx) {
    if (indx + 1 >= this->_capacity) {
        this->resize(this->_capacity * 2);
    }
}

template <typename T>
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

template <typename T>
T& Vetor<T>::operator[](int index) {
    return this->_data[index];
}

template <typename T>
const T& Vetor<T>::operator[](int index) const {
    return this->_data[index];
}

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

template <typename T>
Vetor<T>::~Vetor() {
    delete[] this->_data;
}

#endif