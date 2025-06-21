#ifndef ESCALONADOR_HPP
#define ESCALONADOR_HPP

#include <stdexcept>
#include <iostream>
#include "heap.hpp"

class Escalonador {
    private:
        Heap _eventos;
        int _quantidadeEventos;

    public:
        Escalonador();

        void InsereEvento(Evento& evento, Metricas& metricas);

        Evento RetiraProximoEvento(Metricas& metricas);

        bool Vazio();
};

/**
 * @brief Construtor padrão da classe Escalonador.
 *
 * Inicializa a quantidade de eventos como zero.
 */
Escalonador::Escalonador() : _quantidadeEventos(0) { }

/**
 * @brief Insere um novo evento na lista de eventos do escalonador.
 *
 * Esta função adiciona o evento fornecido à estrutura interna de eventos
 * e incrementa o contador de eventos armazenados.
 *
 * @param evento Referência para o objeto Evento a ser inserido.
 */
void Escalonador::InsereEvento(Evento& evento, Metricas& metricas) {
    // Adiciona o evento ao heap de eventos
    this->_eventos.Inserir(evento);
    this->_quantidadeEventos++;
    metricas.incHeapInsert();
}

/**
 * @brief Remove e retorna o próximo evento da fila de eventos.
 *
 * @throws std::runtime_error Se não houver eventos para retirar.
 * @return Evento O próximo evento retirado da fila.
 */
Evento Escalonador::RetiraProximoEvento(Metricas& metricas) {
    // Verifica se há eventos disponíveis para serem retirados
    if (this->_eventos.Vazio()) {
        throw std::runtime_error("Não há eventos para retirar.");
    }
    
    // Remove o evento do topo da fila de eventos
    // e decrementa a quantidade de eventos
    Evento evento = this->_eventos.Remover();
    this->_quantidadeEventos--;

    metricas.incHeapExtract();
    return evento;
}

/**
 * @brief Verifica se a lista de eventos do escalonador está vazia.
 *
 * @return true se não houver eventos agendados, false caso contrário.
 */
bool Escalonador::Vazio() {
    return this->_eventos.Vazio();
}

#endif