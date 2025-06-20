#ifndef HEAP_HPP
#define HEAP_HPP

#include "utils.hpp"
#include "vetor.hpp"

/*
* Chave da fila de prioridades:
* Dígitos: 13
*
*   1. Tempo: 6 dígitos
*
*   2. ID do pacote: 6 dígitos 
*   ou 
*   2. ID do armazém de origem: 3 dígitos
*   3. ID do armazém de destino: 3 dígitos
*
*   5. Tipo de evento: 1 dígito
*/

class Heap{
    private:
        int _tamanho;
        Vetor<Evento> _data;

        int GetAncestral(int posicao);
        int GetSucessorEsq(int posicao);
        int GetSucessorDir(int posicao);

        void HeapifyPorBaixo(int posicao);
        void HeapifyPorCima(int posicao);

    public:
        Heap();
        Heap(int maxsize);

        void Inserir(Evento evento);
        Evento Remover();        bool Vazio();

};

/**
 * @brief Construtor padrão da classe Heap.
 * 
 * Inicializa o heap com tamanho zero e uma capacidade inicial de 10 elementos.
 */
Heap::Heap(): _tamanho(0), _data(10) {}

/**
 * @brief Construtor da classe Heap com tamanho máximo especificado.
 * 
 * Inicializa o heap com tamanho zero e uma capacidade inicial definida pelo parâmetro maxsize.
 * 
 * @param maxsize Capacidade máxima do heap.
 */
Heap::Heap(int maxsize): _tamanho(0), _data(maxsize) {}

/**
 * @brief Insere um novo evento no heap.
 * 
 * @param evento Evento a ser inserido no heap.
 */
void Heap::Inserir(Evento evento) {
    this->_data.insere(this->_tamanho, evento);

    // Ajusta o tamanho do heap e garante que a propriedade do heap seja mantida
    this->HeapifyPorBaixo(this->_tamanho++);
}

/**
 * @brief Remove o evento com a menor chave do heap.
 * 
 * @return Evento O evento removido do heap, ou um evento inválido se o heap estiver vazio.
 */
Evento Heap::Remover() {
    // Se o heap estiver vazio, retorna um evento inválido
    if (this->Vazio()) {
        return Evento(-1, -1, -1, -1, TipoEvento::CHEGADA_PACOTE);
    }

    // Remove o evento com a menor chave (raiz do heap)
    Evento aux = this->_data[0];

    /*
    * Substitui a raiz do heap pelo último elemento e reduz o tamanho do heap.
    * Em seguida, ajusta a estrutura do heap para manter a propriedade do heap.
    */
    this->_data[0] = _data[this->_tamanho - 1];
    this->_tamanho--;
    HeapifyPorCima(0);
    return aux;
}

/**
 * @brief Verifica se o heap está vazio.
 * 
 * @return true Se o heap estiver vazio.
 * @return false Se o heap contiver elementos.
 */
bool Heap::Vazio() {
    return this->_tamanho == 0;
}

/**
 * @brief Obtém o índice do ancestral de um nó no heap.
 * 
 * @param posicao Índice do nó cujo ancestral será obtido.
 * @return int Índice do ancestral do nó.
 */
int Heap::GetAncestral(int posicao) {
    return (posicao - 1) / 2;
}

/**
 * @brief Obtém o índice do sucessor direito de um nó no heap.
 * 
 * @param posicao Índice do nó cujo sucessor direito será obtido.
 * @return int Índice do sucessor direito do nó.
 */
int Heap::GetSucessorDir(int posicao) {
    return (posicao * 2) + 2;
}

/**
 * @brief Obtém o índice do sucessor esquerdo de um nó no heap.
 * 
 * @param posicao Índice do nó cujo sucessor esquerdo será obtido.
 * @return int Índice do sucessor esquerdo do nó.
 */
int Heap::GetSucessorEsq(int posicao) {
    return (posicao * 2) + 1;
}

/**
 * @brief Reorganiza o heap para manter a propriedade do heap após uma inserção.
 * 
 * Este método é chamado recursivamente para garantir que o elemento na posição especificada
 * esteja na posição correta em relação aos seus ancestrais.
 * 
 * @param posicao Índice do nó a ser reorganizado.
 */
void Heap::HeapifyPorBaixo(int posicao) {
    if (posicao == 0) return;
    
    // Encontra o índice do ancestral
    int ant = this->GetAncestral(posicao);

    /*
    * Verifica se o ancestral é maior que o elemento atual.
    * Se for, troca os elementos e continua a reorganização recursivamente.
    */
    if (this->_data[ant] > this->_data[posicao]) {
        Evento aux = this->_data[ant];
        this->_data[ant] = this->_data[posicao];
        this->_data[posicao] = aux;
    }
    
    this->HeapifyPorBaixo(ant);
}

/**
 * @brief Reorganiza o heap para manter a propriedade do heap após uma remoção.
 * 
 * Este método é chamado recursivamente para garantir que o elemento na posição especificada
 * esteja na posição correta em relação aos seus sucessores.
 * 
 * @param posicao Índice do nó a ser reorganizado.
 */
void Heap::HeapifyPorCima(int posicao) {
    // Encontra os índices dos sucessores esquerdo e direito
    int suD = this->GetSucessorDir(posicao);
    int suE = this->GetSucessorEsq(posicao);

    // Se ambos os sucessores estiverem fora do tamanho do heap, não há nada a fazer
    if (suD > this->_tamanho || suE > this->_tamanho) return;

    // Determina qual dos sucessores tem a menor chave
    int minSu = this->_data[suE] < this->_data[suD] ? suE : suD;

    /*
    * Se o sucessor com a menor chave é menor que o elemento atual,
    * troca-os e continua a reorganização recursivamente.
    */
    if (this->_data[minSu] < this->_data[posicao]) {
        Evento aux = this->_data[minSu];
        this->_data[minSu] = this->_data[posicao];
        this->_data[posicao] = aux;
    }

    this->HeapifyPorCima(minSu);
}

#endif