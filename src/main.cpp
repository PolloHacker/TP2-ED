#include <iomanip>
#include <iostream>
#include <string>
#include "escalonador.hpp"


int main(int argc, char* argv[]) {

    Escalonador escalonador;
    // Inicializa condições de término para False
    
    // Inicializa as variáveis de controle do escalonador
    escalonador.Inicializa();

    while (!escalonador.Vazio()) {
        // Retira o próximo evento do escalonador
        Evento prox_evento = escalonador.RetiraProximoEvento();
        
        double tempo_atual = prox_evento.getTempo();

        switch (prox_evento.getTipo())
        {
        case TipoEvento::POR_POSTAR:
            // Lógica para o evento "POR_POSTAR"
            break;
        case TipoEvento::POSTADO:
            // Lógica para o evento "POSTADO"
            break;
        case TipoEvento::POR_ARMAZENAR:
            // Lógica para o evento "POR_ARMAZENAR"
            break;
        case TipoEvento::ARMAZENADO:
            // Lógica para o evento "ARMAZENADO"
            break;
        case TipoEvento::EM_ROTA:
            // Lógica para o evento "EM_ROTA"
            break;
        case TipoEvento::ENTREGUE:
            // Lógica para o evento "ENTREGUE"
            break;
        default:
            break;
        }
    }

    // Gerar relatório de estatísticas
    escalonador.Finaliza();

    return (0);
}