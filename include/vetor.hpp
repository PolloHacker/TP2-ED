#ifndef VETOR_HPP
#define VETOR_HPP

#include <stdexcept>

template <typename T>
class Vetor {
    private:
        T* _data;
        int _size;
        int _capacity;

        void resize();

    public:
        Vetor(int size);
        Vetor(const Vetor& other);
        ~Vetor();
        
        void shouldResize();
        
        T& operator[](int index);
        const T& operator[](int index) const;

        Vetor<T>& operator=(const Vetor<T>& other);
};

#include "vetor.tpp"

#endif