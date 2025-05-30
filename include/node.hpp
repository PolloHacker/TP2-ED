#ifndef NODE_HPP
#define NODE_HPP


template <typename T>
class Node {
    public:
        Node();
        Node(T val);
        ~Node();

        T GetData();
        Node<T>* GetNext();

        void SetData(T val);
        void SetNext(Node<T>* obj);

    private:
        
        T _data;
        Node<T>* _next;
};

#endif