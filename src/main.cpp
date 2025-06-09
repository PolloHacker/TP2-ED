#include <fstream>
#include <iostream>
#include <string>
#include "vetor.hpp"
#include "pacote.hpp"
#include "escalonador.hpp"
#include "transporte.hpp"



void leArquivo(std::string nomeArquivo, Transporte& rotas, Escalonador& escalonador, Lista<Pacote<int>>& pacotes) {
    int i, j;
    std::ifstream arquivo(nomeArquivo.c_str());
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << nomeArquivo << std::endl;
        exit(1);
    }

    int capacidadeTransporte, latenciaTransporte, intervaloTransportes, custoRemocao;
    int numeroArmazens, numeroPacotes;

    arquivo >> capacidadeTransporte;
    arquivo >> latenciaTransporte;
    arquivo >> intervaloTransportes;
    arquivo >> custoRemocao;
    arquivo >> numeroArmazens;

    for (i = 0; i < numeroArmazens; ++i) {
        rotas.adicionaArmazem(i);
    }

    // Lê a matriz de conexões entre armazéns
    for (i = 0; i < numeroArmazens; ++i) {
        for (j = 0; j < numeroArmazens; ++j) {
            int conexao;
            arquivo >> conexao;
            if (conexao > 0) {
                rotas.conectaArmazens(i, j);
            }
        }
    }

    arquivo >> numeroPacotes;

    for (int i = 0; i < numeroPacotes; ++i) {
        Pacote<int> p;
        std::string tmp;
        int id, origem, destino;
        double tempoPostagem;

        arquivo >> tempoPostagem >> tmp >> id >> tmp >> origem >> tmp >> destino;

        Evento ev = Evento(tempoPostagem, id, TipoEvento::POSTAGEM);

        escalonador.InsereEvento(ev);
    }
    arquivo.close();

}


int main(int argc, char* argv[]) {

    Escalonador escalonador;
    Transporte rotas;
    Lista<Pacote<int>> pacotes;

    // Verifica se o nome do arquivo foi passado como argumento
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <nome_do_arquivo>" << std::endl;
        return 1;
    }

    // Lê o arquivo de entrada
    std::string nomeArquivo = argv[1];
    leArquivo(nomeArquivo, rotas, escalonador, pacotes);
    
    // Inicializa as variáveis de controle do escalonador
    escalonador.Inicializa();

    while (!escalonador.Vazio()) {
        // Retira o próximo evento do escalonador
        Evento prox_evento = escalonador.RetiraProximoEvento();
        
        double tempo_atual = prox_evento.getTempo();

        switch (prox_evento.getTipo())
        {
        case TipoEvento::POSTAGEM:
            // Lógica para o evento "POSTAGEM"
            break;
        case TipoEvento::TRANSPORTE:
            // Lógica para o evento "TRANSPORTE"
            break;
        default:
            break;
        }
    }

    // Gerar relatório de estatísticas
    escalonador.Finaliza();

    return (0);
}