#include <iomanip>
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

        Evento ev(tempoPostagem, i, origem, destino, TipoEvento::PACOTE);
        escalonador.InsereEvento(ev);

        pacotes.insere(i, p);
    }
    arquivo.close();
}


void handlePostagem(
    Evento prox_evento, Escalonador& Escalonador,
    Vetor<Armazem>& armazens, Vetor<Pacote<int>>& pacotes, Vetor<int>& custos) {
    int idPacote = prox_evento.getIdPacote();
    int tempoAtual = prox_evento.getTempo();
    int armazemAtual = pacotes[idPacote].getIdArmazemAtual() - 1;
    int secaoAtual = pacotes[idPacote].getIdSecaoAtual() - 1;

    try {
        pacotes[idPacote];
    } catch (const std::out_of_range& e) {
        std::cerr << "Erro: Pacote com ID " << idPacote << " não encontrado." << std::endl;
        return;
    }

    pacotes[idPacote].setEstado(EstadoPacote::POSTADO);

    std::cout << std::setfill('0') 
              << std::setw(7) << tempoAtual << " pacote "
              << std::setw(3) << idPacote << " armazenado em "
              << std::setw(3) << armazemAtual << " na secao "
              << std::setw(3) << secaoAtual
              << std::endl;
    
    Evento novoEvento(
        tempoAtual + 
        custos[static_cast<int>(CustoEvento::INTERVALO_TRANSPORTE)] + 
        custos[static_cast<int>(CustoEvento::CUSTO_REMOVER_PACOTE)],
        idPacote,
        pacotes[idPacote].getIdArmazemAtual(),
        pacotes[idPacote].getRota().GetElemPos(2)->GetData(), // Próximo armazém na rota
        TipoEvento::REMOCAO
    );

    Escalonador.InsereEvento(novoEvento);
}

void handleRemocao(
    Evento prox_evento, Escalonador& Escalonador,
    Vetor<Armazem>& armazens, Vetor<Pacote<int>>& pacotes, Vetor<int>& custos) {

    int idPacote = prox_evento.getIdPacote();
    int tempoAtual = prox_evento.getTempo();
    int armazemAtual = pacotes[idPacote].getIdArmazemAtual() - 1;
    int secaoAtual = pacotes[idPacote].getIdSecaoAtual() - 1;

    try {
        pacotes[idPacote];
    } catch (const std::out_of_range& e) {
        std::cerr << "Erro: Pacote com ID " << idPacote << " não encontrado." << std::endl;
        return;
    }
    
    pacotes[idPacote].setEstado(EstadoPacote::EM_SEPARACAO);

    std::cout << std::setfill('0') 
              << std::setw(7) << tempoAtual << " pacote "
              << std::setw(3) << idPacote << " removido de "
              << std::setw(3) << armazemAtual << " na secao " 
              << std::setw(3) << secaoAtual
              << std::endl;
    
    Lista<int> rota = pacotes[idPacote].getRota();

    Evento novoEvento(
        tempoAtual,
        idPacote,
        pacotes[idPacote].getIdArmazemAtual(),
        rota.GetElemPos(2)->GetData(), // Próximo armazém na rota
        TipoEvento::TRANSPORTE
    );

    Escalonador.InsereEvento(novoEvento);
}

void handleTransporte(
    Evento prox_evento, Escalonador& Escalonador,
    Vetor<Armazem>& armazens, Vetor<Pacote<int>>& pacotes, Vetor<int>& custos) {

    int idPacote = prox_evento.getIdPacote();
    int tempoAtual = prox_evento.getTempo();
    int armazemAtual = pacotes[idPacote].getIdArmazemAtual() - 1;
    int secaoAtual = pacotes[idPacote].getIdSecaoAtual() - 1;

    try {
        pacotes[idPacote];
    } catch (const std::out_of_range& e) {
        std::cerr << "Erro: Pacote com ID " << idPacote << " não encontrado." << std::endl;
        return;
    }
    
    pacotes[idPacote].setEstado(EstadoPacote::EM_TRANSPORTE);

    std::cout << std::setfill('0') 
              << std::setw(7) << tempoAtual << " pacote "
              << std::setw(3) << idPacote << " em transito de "
              << std::setw(3) << armazemAtual << " para " 
              << std::setw(3) << secaoAtual
              << std::endl;
    
    Evento novoEvento(
        tempoAtual + custos[static_cast<int>(CustoEvento::LATENCIA_TRANSPORTE)],
        idPacote,
        pacotes[idPacote].getIdArmazemAtual(),
        pacotes[idPacote].getRota().GetElemPos(2)->GetData(), // Próximo armazém na rota
        TipoEvento::ENTREGA
    );

    Escalonador.InsereEvento(novoEvento);
}

void handleRearmazenamento(
    Evento prox_evento, Escalonador& Escalonador,
    Vetor<Armazem>& armazens, Vetor<Pacote<int>>& pacotes, Vetor<int>& custos) {

    int idPacote = prox_evento.getIdPacote();
    int tempoAtual = prox_evento.getTempo();
    int armazemAtual = pacotes[idPacote].getIdArmazemAtual() - 1;
    int secaoAtual = pacotes[idPacote].getIdSecaoAtual() - 1;

    try {
        pacotes[idPacote];
    } catch (const std::out_of_range& e) {
        std::cerr << "Erro: Pacote com ID " << idPacote << " não encontrado." << std::endl;
        return;
    }

    pacotes[idPacote].setEstado(EstadoPacote::EM_SEPARACAO);

    std::cout << std::setfill('0') 
              << std::setw(7) << tempoAtual << " pacote "
              << std::setw(3) << idPacote << " rearmazenado em "
              << std::setw(3) << armazemAtual << " na secao " 
              << std::setw(3) << secaoAtual
              << std::endl;

    Evento novoEvento(
        tempoAtual,
        idPacote,
        pacotes[idPacote].getIdArmazemAtual(),
        pacotes[idPacote].getRota().GetElemPos(2)->GetData(), // Próximo armazém na rota
        TipoEvento::REARMAZENAMENTO
    );

}

void handleEntrega(
    Evento prox_evento, Escalonador& Escalonador,
    Vetor<Armazem>& armazens, Vetor<Pacote<int>>& pacotes, Vetor<int>& custos) {

    int idPacote = prox_evento.getIdPacote();
    int tempoAtual = prox_evento.getTempo();

    try {
        pacotes[idPacote];
    } catch (const std::out_of_range& e) {
        std::cerr << "Erro: Pacote com ID " << idPacote << " não encontrado." << std::endl;
        return;
    }

    pacotes[idPacote].setEstado(EstadoPacote::EM_TRANSPORTE);
    int armazemAtual = pacotes[idPacote].getIdArmazemAtual();
    int destino = pacotes[idPacote].getRota().GetElemPos(pacotes[idPacote].getRota().GetTam())->GetData();

    if (armazemAtual == destino) {
        pacotes[idPacote].setEstado(EstadoPacote::ENTREGUE);

        std::cout << std::setfill('0') 
                  << std::setw(7) << tempoAtual << " pacote "
                  << std::setw(3) << idPacote << " entregue em "
                  << std::setw(3) << destino - 1 << std::endl;
    } else {
        int proximaPos = 0;
        Lista<int> rota = pacotes[idPacote].getRota();

        for (int k = 1; k <= rota.GetTam(); ++k) {
            if (rota.GetElemPos(k)->GetData() == armazemAtual) {
                proximaPos = k + 1;
                break;
            }
        }
        if (proximaPos > 0 && proximaPos <= rota.GetTam()) {
            int proximoArmazem = rota.GetElemPos(proximaPos)->GetData();
            pacotes[idPacote].setIdArmazemAtual(proximoArmazem);

            Evento novoEvento(
                tempoAtual + custos[static_cast<int>(CustoEvento::LATENCIA_TRANSPORTE)],
                idPacote,
                proximoArmazem,
                destino,
                TipoEvento::ENTREGA
            );

            Escalonador.InsereEvento(novoEvento);
        }
    }

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
        case TipoEvento::PACOTE:
            handlePostagem(prox_evento, escalonador, armazens, pacotes, custos);
            break;
        case TipoEvento::REMOCAO:
            handleRemocao(prox_evento, escalonador, armazens, pacotes, custos);
            break;
        case TipoEvento::TRANSPORTE:
            handleTransporte(prox_evento, escalonador, armazens, pacotes, custos);
            break;
        case TipoEvento::REARMAZENAMENTO:
            handleRearmazenamento(prox_evento, escalonador, armazens, pacotes, custos);
            break;
        case TipoEvento::ENTREGA:
            handleEntrega(prox_evento, escalonador, armazens, pacotes, custos);
            break;
        default:
            break;
        }
    }

    // Gerar relatório de estatísticas
    escalonador.Finaliza();

    return (0);
}