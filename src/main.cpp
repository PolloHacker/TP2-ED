#include <iomanip>
#include <fstream>
#include <iostream>
#include <string>
#include "vetor.hpp"
#include "pacote.hpp"
#include "escalonador.hpp"
#include "transporte.hpp"


void escalonaTransportes(Escalonador& escalonador, 
    Vetor<Armazem>& armazens, int numArmazens, int tempoAtual) {

    for (int origem = 1; origem <= numArmazens; ++origem) {
        for (int destino = 1; destino <= numArmazens; ++destino) {
            if (origem != destino) {
                int cooldown = armazens[origem].getCooldown(destino);
                Evento novoTransporte(tempoAtual + cooldown, -1, origem, destino, TipoEvento::TRANSPORTE);
                escalonador.InsereEvento(novoTransporte);
            }
        }
    }
}

void leArquivo(std::string nomeArquivo, Transporte& rotas, 
    Vetor<Armazem>& armazens, Escalonador& escalonador, 
    Vetor<Pacote<int>>& pacotes, Vetor<int>& custos) {

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

    custos.insere(0, capacidadeTransporte);
    custos.insere(1, latenciaTransporte);
    custos.insere(2, intervaloTransportes);
    custos.insere(3, custoRemocao);
    
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
                armazens[i].adicionaVizinho(j);
                armazens[i].setCooldown(j, latenciaTransporte);
                armazens[i].setCapacidade(j, capacidadeTransporte);
            }
        }
    }

    arquivo >> numeroPacotes;

    for (int i = 0; i < numeroPacotes; ++i) {
        std::string tmp;
        int id, origem, destino, tempoPostagem;

        arquivo >> tempoPostagem >> tmp >> id >> tmp >> origem >> tmp >> destino;

        origem++;
        destino++;

        Pacote<int> p(id, "", "", 0);
        Lista<int> rota = rotas.calculaRota(origem, destino);
        p.setRota(rota);

        std::cout << "Rota calculada tem tamanho " << rota.GetTam() << ": ";
        for (int k = 1; k <= rota.GetTam(); ++k) {
            std::cout << rota.GetElemPos(k)->GetData() << " ";
        }

        p.setIdArmazemAtual(origem);
        p.setIdSecaoAtual(rota.GetElemPos(2)->GetData());

        std::cout << "Pacote ID: " << id 
                  << ", Origem: " << origem 
                  << ", Destino: " << destino 
                  << ", Tempo de Postagem: " << tempoPostagem 
                  << ", Armazem Atual: " << p.getIdArmazemAtual()
                  << ", Seção Atual: " << p.getIdSecaoAtual()
                  << std::endl;

        if (i == 0) {
            escalonaTransportes(escalonador, armazens, numeroArmazens, tempoPostagem);
        }

        Evento ev(tempoPostagem, i, origem, destino, TipoEvento::CHEGADA_PACOTE);
        escalonador.InsereEvento(ev);

        pacotes.insere(i, p);
    }
    arquivo.close();
}


void handleChegadaPacote(
    Evento evento, Escalonador& escalonador,
    Vetor<Armazem>& armazens, Vetor<Pacote<int>>& pacotes, Vetor<int>& custos) {
    int idPacote = evento.getIdPacote();
    int tempoAtual = evento.getTempo();
    int armazemAntigo = pacotes[idPacote].getIdArmazemAtual() - 1;
    int secaoAntiga = pacotes[idPacote].getIdSecaoAtual() - 1;

    try {
        pacotes[idPacote];
    } catch (const std::out_of_range& e) {
        std::cerr << "Erro: Pacote com ID " << idPacote << " não encontrado." << std::endl;
        return;
    }

    if (pacotes[idPacote].getIdArmazemAtual() == pacotes[idPacote].getRota().GetElemPos(pacotes[idPacote].getRota().GetTam())->GetData()) {
        pacotes[idPacote].setEstado(EstadoPacote::ENTREGUE);

        std::cout << std::setfill('0')
                  << std::setw(7) << tempoAtual << " pacote "
                  << std::setw(3) << idPacote << " entregue em "
                  << std::setw(3) << armazemAntigo << std::endl;

        return;
    } else {
        Lista<int> rota = pacotes[idPacote].getRota();
        int armazemAtual;

        if (rota.GetTam() > 1) {
            rota.RemovePos(1); // Remove o armazém atual (primeiro da rota)
            pacotes[idPacote].setRota(rota);

            armazemAtual = rota.GetElemPos(1)->GetData();
            pacotes[idPacote].setIdArmazemAtual(armazemAtual);
            if (rota.GetTam() > 1) {
                pacotes[idPacote].setIdSecaoAtual(rota.GetElemPos(2)->GetData());
            } else {
                pacotes[idPacote].setIdSecaoAtual(armazemAtual);
            }
        }
        
        armazens[armazemAtual].armazenaPacote(armazemAtual, idPacote);
        
    }

    // Agendar próximo evento de transporte para este pacote
    Evento novoEvento(
        tempoAtual,
        idPacote,
        pacotes[idPacote].getIdArmazemAtual(),
        pacotes[idPacote].getIdSecaoAtual(),
        TipoEvento::TRANSPORTE
    );

    pacotes[idPacote].setEstado(EstadoPacote::POSTADO);

    std::cout << std::setfill('0') 
              << std::setw(7) << tempoAtual << " pacote "
              << std::setw(3) << idPacote << " armazenado em "
              << std::setw(3) << armazemAntigo << " na secao "
              << std::setw(3) << secaoAntiga
              << std::endl;
    
    

    escalonador.InsereEvento(novoEvento);
}

void handleTransporte(
    Evento prox_evento, Escalonador& Escalonador,
    Vetor<Armazem>& armazens, Vetor<Pacote<int>>& pacotes, Vetor<int>& custos) {

}

int main(int argc, char* argv[]) {

    Escalonador escalonador;
    Transporte rotas;
    Vetor<Armazem> armazens(1);
    Vetor<Pacote<int>> pacotes(1);
    Vetor<int> custos(5);

    // Verifica se o nome do arquivo foi passado como argumento
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <nome_do_arquivo>" << std::endl;
        return 1;
    }

    // Lê o arquivo de entrada
    std::string nomeArquivo = argv[1];
    leArquivo(nomeArquivo, rotas, armazens, escalonador, pacotes, custos);

    rotas.imprimeRede();
    
    // Inicializa as variáveis de controle do escalonador
    escalonador.Inicializa();

    while (!escalonador.Vazio()) {
        // Retira o próximo evento do escalonador
        Evento prox_evento = escalonador.RetiraProximoEvento();
        
        // double tempo_atual = prox_evento.getTempo();, Vetor<Pacote>& pacotes

        switch (prox_evento.getTipoEvento())
        {
        case TipoEvento::CHEGADA_PACOTE:
            handleChegadaPacote(prox_evento, escalonador, armazens, pacotes, custos);
            break;
        case TipoEvento::TRANSPORTE:
            handleTransporte(prox_evento, escalonador, armazens, pacotes, custos);
            break;
        default:
            break;
        }
    }

    // Gerar relatório de estatísticas
    escalonador.Finaliza();

    return (0);
}