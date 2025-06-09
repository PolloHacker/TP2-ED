#ifndef VETOR_HPP
#define VETOR_HPP

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
};

#endif