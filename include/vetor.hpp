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

#include "vetor.tpp"

#endif