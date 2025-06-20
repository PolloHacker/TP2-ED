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


/**
 * @brief Construtor padrão da classe Node.
 *
 * Inicializa o nó com um valor padrão do tipo T e o ponteiro _next como nullptr.
 */
template <typename T>
Node<T>::Node() : _data(T()), _next(nullptr) {}

/**
 * @brief Construtor da classe Node com um valor específico.
 *
 * Inicializa o nó com o valor fornecido e o ponteiro _next como nullptr.
 *
 * @param val Valor a ser atribuído ao nó.
 */
template <typename T>
Node<T>::Node(T val) : _data(val), _next(nullptr) {}

/**
 * @brief Construtor de cópia da classe Node.
 *
 * Cria um novo nó copiando o valor do nó fornecido como parâmetro.
 * O ponteiro _next é inicializado como nullptr.
 *
 * @param other Referência constante para o nó a ser copiado.
 */
template <typename T>
Node<T>::Node(const Node<T>& other) : _data(other._data), _next(nullptr) {}

/**
 * @brief Operador de atribuição para a classe Node.
 *
 * Copia o valor do nó fornecido como parâmetro para o nó atual,
 * garantindo que não haja autoatribuição. O ponteiro _next é inicializado como nullptr.
 *
 * @param other Referência constante para o nó a ser copiado.
 * @return Referência para o próprio objeto Node após a atribuição.
 */
template <typename T>
Node<T>& Node<T>::operator=(const Node<T>& other) {
    if (this != &other) {
        this->_data = other._data;
        this->_next = nullptr;
    }
    return *this;
}

/**
 * @brief Obtém uma referência ao dado armazenado no nó.
 *
 * Este método retorna uma referência ao valor armazenado no nó,
 * permitindo modificações diretas no valor.
 *
 * @return Referência ao dado do nó.
 */
template <typename T>
T& Node<T>::GetDataRef() {
    return this->_data;
}

/**
 * @brief Obtém uma referência ao dado armazenado no nó.
 *
 * Este método retorna uma referência ao valor armazenado no nó,
 * permitindo modificações diretas no valor.
 *
 * @return Referência ao dado do nó.
 */
template <typename T>
T& Node<T>::GetData() {
    return this->_data;
}

/**
 * @brief Obtém o próximo nó na lista.
 *
 * Este método retorna um ponteiro para o próximo nó na lista encadeada.
 *
 * @return Ponteiro para o próximo nó, ou nullptr se não houver próximo nó.
 */
template <typename T>
Node<T>* Node<T>::GetNext() {
    return this->_next;
}

/**
 * @brief Define o valor armazenado no nó.
 *
 * Este método atribui um novo valor ao dado do nó.
 *
 * @param val Novo valor a ser atribuído ao nó.
 */
template <typename T>
void Node<T>::SetData(T val) {
    this->_data = val;
}

/**
 * @brief Define o próximo nó na lista.
 *
 * Este método atribui um novo nó como o próximo nó na lista encadeada.
 *
 * @param obj Ponteiro para o próximo nó a ser definido.
 */
template <typename T>
void Node<T>::SetNext(Node<T>* obj) {
    this->_next = obj;
}

#endif