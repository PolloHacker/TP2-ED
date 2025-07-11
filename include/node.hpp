#ifndef NODE_HPP
#define NODE_HPP


template <typename T>
class Node {
    private:
        T _data;
        Node<T>* _next;
    
    public:
        Node();
        Node(T val);
        
        Node(const Node<T>& other);
        Node<T>& operator=(const Node<T>& other);

        T& GetDataRef();
        T& GetData();
        Node<T>* GetNext();

        void SetData(T val);
        void SetNext(Node<T>* obj);

};

#include "node.tpp"

#endif