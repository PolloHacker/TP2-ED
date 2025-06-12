#include <fstream>
#include <iostream>
#include <string>
#include "vetor.hpp"
#include "pacote.hpp"
#include "escalonador.hpp"
#include "transporte.hpp"


void escalonaTransportes(Escalonador& escalonador, 
    Vetor<Armazem>& armazens, int numArmazens, int tempoAtual) {

    // Escalona eventos de transporte apenas para conexões existentes
    for (int origem = 1; origem <= numArmazens; ++origem) {
        Lista<int> vizinhos = armazens[origem].getVizinhos();
        for (int i = 1; i <= vizinhos.GetTam(); ++i) {
            int destino = vizinhos.GetElemPos(i)->GetData();
            int cooldown = armazens[origem].getCooldown(destino);

            std::cout << "Escalonando transporte de " 
                      << origem << " para " 
                      << destino 
                      << " no tempo " << tempoAtual + cooldown << std::endl;

            Evento novoTransporte(tempoAtual + cooldown, -1, origem, destino, TipoEvento::TRANSPORTE);
            escalonador.InsereEvento(novoTransporte);
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
                armazens[i].setCooldown(j, intervaloTransportes);
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
    Evento evento, Vetor<Armazem>& armazens, 
    Vetor<Pacote<int>>& pacotes) {

    int idPacote = evento.getIdPacote();
    int tempoAtual = evento.getTempo();
    int armazemAntigo = pacotes[idPacote].getIdArmazemAtual() - 1;
    // int secaoAntiga = pacotes[idPacote].getIdSecaoAtual() - 1;

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
        int armazemAtual = pacotes[idPacote].removeArmazemAtualDaRota();
        int secaoAtual = pacotes[idPacote].getProximoArmazemRota();
        
        pacotes[idPacote].setIdArmazemAtual(armazemAtual);
        pacotes[idPacote].setIdSecaoAtual(secaoAtual);
        armazens[armazemAtual].armazenaPacote(secaoAtual, idPacote);

        pacotes[idPacote].setEstado(EstadoPacote::POSTADO);

        std::cout << std::setfill('0') 
                << std::setw(7) << tempoAtual << " pacote "
                << std::setw(3) << idPacote << " armazenado em "
                << std::setw(3) << armazemAtual - 1 << " na secao "
                << std::setw(3) << secaoAtual - 1
                << std::endl;
    }
}

void handleTransporte(
    Evento evento, Escalonador& Escalonador,
    Vetor<Armazem>& armazens, Vetor<Pacote<int>>& pacotes, Vetor<int>& custos) {

    Vetor<int> armazensEvento = evento.getArmazens();

    int tempoAtual = evento.getTempo();
    int latencia = custos[static_cast<int>(CustoEvento::LATENCIA_TRANSPORTE)];
    int remocao = custos[static_cast<int>(CustoEvento::CUSTO_REMOVER_PACOTE)];

    Armazem& armazemOrigem = armazens[armazensEvento[0]];

    Pilha<int> pacotesRearmazenar = armazemOrigem.adicionaPacotesParaTransporte(armazensEvento[1], tempoAtual, remocao);
    Lista<int> pacotesParaTransportar = armazemOrigem.getTransportesPorVizinho(armazensEvento[1]);

    if (pacotesParaTransportar.GetTam() > 0) {
        for (int idx = 1; idx <= pacotesParaTransportar.GetTam(); ++idx) {

            int idPacote = pacotesParaTransportar.GetElemPos(idx)->GetData();
            pacotes[idPacote].setEstado(EstadoPacote::EM_TRANSPORTE);
            pacotes[idPacote].setIdArmazemAtual(armazensEvento[1]);

            Evento chegadaEvento(
                tempoAtual + latencia,
                idPacote,
                armazensEvento[1],
                pacotes[idPacote].getIdSecaoAtual(),
                TipoEvento::CHEGADA_PACOTE
            );
            Escalonador.InsereEvento(chegadaEvento);

            std::cout << std::setfill('0')
                        << std::setw(7) << tempoAtual << " pacote "
                        << std::setw(3) << idPacote << " em transito de "
                        << std::setw(3) << armazensEvento[0] - 1 << " para "
                        << std::setw(3) << armazensEvento[1] - 1 << std::endl;
        }
    }

    // Escalona o próximo evento de transporte para esta rota
    int timeDiff = (pacotesParaTransportar.GetTam() + pacotesRearmazenar.GetTam()) * remocao;
    tempoAtual -= timeDiff;

    Evento proximoTransporte(
        tempoAtual + custos[2], // intervaloTransportes
        -1,
        armazensEvento[0],
        armazensEvento[1],
        TipoEvento::TRANSPORTE
    );
    Escalonador.InsereEvento(proximoTransporte);

    // Rearmazenar pacotes após o transporte
    armazemOrigem.rearmazenarPacotes(armazensEvento[1], pacotesRearmazenar, tempoAtual + timeDiff);
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

        // Verifica se todos os pacotes foram entregues
        bool todosEntregues = true;
        for (int i = 0; i < pacotes.getSize(); ++i) {
            if (pacotes[i].getEstado() != EstadoPacote::ENTREGUE) {
                todosEntregues = false;
                break;
            }
        }

        if (todosEntregues) {
            break;
        }

        switch (prox_evento.getTipoEvento())
        {
        case TipoEvento::CHEGADA_PACOTE:
            handleChegadaPacote(prox_evento, armazens, pacotes);
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