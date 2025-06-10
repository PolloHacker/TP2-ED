#ifndef LISTA_HPP
#define LISTA_HPP

#include <iostream>
#include <exception>
#include <string>
#include "node.hpp"

template <typename T>
class Lista {
    private:
        Node<T>* _lista;
        int _tam;
        
    public:
        Lista();
        ~Lista();

        Node<T>* Posiciona(int pos);
        void InsereInicio(Node<T>& obj);
        void InserePos(int pos, Node<T>& obj);
        void InsereFim(Node<T>& obj);
        
        Node<T> RemovePos(int pos);
        Node<T>* GetElemPos(int pos);

        int GetTam();
        Node<T>* _head;
        Node<T>* _tail;
        

};

#include "lista.tpp"

#endif