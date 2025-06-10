#include <fstream>
#include <iostream>
#include <string>
#include "vetor.hpp"
#include "pacote.hpp"
#include "escalonador.hpp"
#include "transporte.hpp"


void leArquivo(
    std::string nomeArquivo, Transporte& rotas, 
    Vetor<Armazem>& armazens, Escalonador& escalonador, 
    Vetor<Pacote<int>>& pacotes) {

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
    
    for (i = 1; i <= numeroArmazens; ++i) {
        Armazem armazemToIns = rotas.adicionaArmazem(i);
        armazens.insere(i, armazemToIns);
    }

    for (i = 1; i <= numeroArmazens; ++i) {
        for (j = 1; j <= numeroArmazens; ++j) {
            int conexao;
            arquivo >> conexao;
            if (conexao > 0) {
                rotas.conectaArmazens(i, j);
            }
        }
    }

    arquivo >> numeroPacotes;

    for (int i = 0; i < numeroPacotes; ++i) {
        std::string tmp;
        int id, origem, destino, tempoPostagem;

        arquivo >> tempoPostagem >> tmp >> id >> tmp >> origem >> tmp >> destino;

        Pacote<int> p(id, "", "", 0);
        p.setRota(rotas.calculaRota(origem, destino));

        Evento ev(tempoPostagem, id, TipoEvento::PACOTE);
        escalonador.InsereEvento(ev);

        pacotes.insere(i, p);
    }
    arquivo.close();
}


int main(int argc, char* argv[]) {

    Escalonador escalonador;
    Transporte rotas;
    Vetor<Armazem> armazens(1);
    Vetor<Pacote<int>> pacotes(1);

    // Verifica se o nome do arquivo foi passado como argumento
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <nome_do_arquivo>" << std::endl;
        return 1;
    }

    // Lê o arquivo de entrada
    std::string nomeArquivo = argv[1];
    leArquivo(nomeArquivo, rotas, armazens, escalonador, pacotes);

    rotas.imprimeRede();
    
    // Inicializa as variáveis de controle do escalonador
    escalonador.Inicializa();

    while (!escalonador.Vazio()) {
        // Retira o próximo evento do escalonador
        Evento prox_evento = escalonador.RetiraProximoEvento();
        
        // double tempo_atual = prox_evento.getTempo();

        switch (prox_evento.getTipoEvento())
        {
        case TipoEvento::PACOTE:
            // Lógica para o evento "POSTAGEM"
            std::cout << "Evento de postagem do pacote ID: " 
                      << prox_evento.getIdPacote() << " no tempo: " 
                      << prox_evento.getTempo() << std::endl;
            // Aqui você pode implementar a lógica para processar o pacote
            break;
        case TipoEvento::TRANSPORTE:
            // Lógica para o evento "TRANSPORTE"
            std::cout << "Evento de transporte entre armazéns: " 
                      << prox_evento.getArmazens()[0] << " e " 
                      << prox_evento.getArmazens()[1] << " no tempo: " 
                      << prox_evento.getTempo() << std::endl;
            // Aqui você pode implementar a lógica para processar o transporte
            break;
        default:
            break;
        }
    }

    // Gerar relatório de estatísticas
    escalonador.Finaliza();

    return (0);
}