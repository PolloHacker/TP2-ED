#ifndef LISTA_HPP
#define LISTA_HPP

#include <iostream>
#include <exception>
#include <string>
#include "node.hpp"

template <typename T>
class Lista {
    private:
        int _tam;
        
    public:
        Lista();
        Lista(const Lista<T>& other);
        Lista<T>& operator=(const Lista<T>& other);
        ~Lista();

        Node<T>* Posiciona(int pos);
        void InsereInicio(T& obj);
        void InserePos(int pos, T& obj);
        void InsereFim(T& obj);
        
        T RemovePos(int pos);
        Node<T>* GetElemPos(int pos);

        int GetTam();
        Node<T>* _head;
        Node<T>* _tail;
        

};

#include "lista.tpp"

#endif