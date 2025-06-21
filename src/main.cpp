#include <fstream>
#include <iostream>
#include <string>
#include "vetor.hpp"
#include "pacote.hpp"
#include "escalonador.hpp"
#include "transporte.hpp"


Metricas metricas;

/**
 * @brief Escalona eventos de transporte entre armazéns conectados.
 *
 * @param escalonador Referência para o objeto responsável por gerenciar os eventos.
 * @param armazens Vetor contendo todos os armazéns.
 * @param numArmazens Número total de armazéns.
 * @param tempoAtual Tempo atual do sistema, utilizado como base para o agendamento dos eventos.
 */
void escalonaTransportes(Escalonador& escalonador, 
    Vetor<Armazem>& armazens, int numArmazens, int tempoAtual) {

    // Escalona eventos de transporte apenas para conexões existentes
    for (int origem = 1; origem <= numArmazens; ++origem) {
        Lista<int> vizinhos = armazens[origem].getVizinhos();
        for (int i = 1; i <= vizinhos.GetTam(); ++i) {
            int destino = vizinhos.GetElemPos(i)->GetData();
            int cooldown = armazens[origem].getCooldown(destino);

            Evento novoTransporte(tempoAtual + cooldown, -1, origem - 1, destino - 1, TipoEvento::TRANSPORTE);
            escalonador.InsereEvento(novoTransporte, metricas);
        }
    }
}

/**
 * @brief Lê os dados de um arquivo e inicializa as estruturas principais do sistema de transporte.
 *
 * @param nomeArquivo Nome do arquivo de entrada a ser lido.
 * @param rotas Referência para o objeto Transporte, responsável pelas rotas e conexões entre armazéns.
 * @param armazens Vetor de Armazem, onde serão inseridos os armazéns lidos do arquivo.
 * @param escalonador Referência para o objeto Escalonador, utilizado para agendar eventos de transporte.
 * @param pacotes Vetor de Pacote<int>, onde serão inseridos os pacotes lidos do arquivo.
 * @param custos Vetor de int, onde serão inseridos os custos e parâmetros globais do sistema.
 *
 * @note Em caso de erro ao abrir o arquivo, a função imprime uma mensagem de erro e encerra o programa.
 */
void leArquivo(std::string nomeArquivo, Transporte& rotas, 
    Vetor<Armazem>& armazens, Escalonador& escalonador, 
    Vetor<Pacote<int>>& pacotes, Vetor<int>& custos) {

    int i, j, k;
    std::ifstream arquivo(nomeArquivo.c_str());
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << nomeArquivo << std::endl;
        exit(1);
    }

    // Lê os parâmetros iniciais do arquivo
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
    
    // Cria a topologia de armazéns e suas conexões
    for (i = 1; i <= numeroArmazens; ++i) {
        Armazem armazemToIns = rotas.adicionaArmazem(i);
        armazens.insere(i, armazemToIns);

        if (i % 5 == 0) metricas.updatePeakMemory();
    }

    for (i = 1; i <= numeroArmazens; ++i) {
        for (j = 1; j <= numeroArmazens; ++j) {
            int conexao;
            arquivo >> conexao;
            if (conexao > 0) {
                rotas.conectaArmazens(i, j, conexao);
                armazens[i].adicionaVizinho(j);
                armazens[i].setCooldown(j, intervaloTransportes);
                armazens[i].setCapacidade(j, capacidadeTransporte);
            }
        }

        if (i % 5 == 0) metricas.updatePeakMemory();
    }

    arquivo >> numeroPacotes;

    /*
    * Lê os pacotes do arquivo e os insere no vetor de pacotes.
    * Cada pacote é associado a uma rota calculada entre o armazém de origem e o de destino. 
    */
    for (k = 0; k < numeroPacotes; ++k) {
        std::string tmp;
        int id, origem, destino, tempoPostagem;

        arquivo >> tempoPostagem >> tmp >> id >> tmp >> origem >> tmp >> destino;

        origem++;
        destino++;

        Pacote<int> p(id, "", "", 0);
        Lista<int> rota = rotas.calculaRotaComPeso(origem, destino);
        p.setRota(rota);

        p.setIdArmazemAtual(origem);
        p.setIdSecaoAtual(rota.GetElemPos(2)->GetData());

        // Se for a primeira postagem, escalona os transportes entre todas as conexões
        if (k == 0) {
            escalonaTransportes(escalonador, armazens, numeroArmazens, tempoPostagem);
        }

        // Cria o evento de chegada do pacote no armazém de origem e o insere no escalonador
        Evento ev(tempoPostagem, k, origem, destino, TipoEvento::CHEGADA_PACOTE);
        escalonador.InsereEvento(ev, metricas);

        pacotes.insere(k, p);

        // Measure memory growth as packages and events are added
        if (k % 10 == 0) metricas.updatePeakMemory();
    }
    arquivo.close();
}


/**
 * @brief Manipula o evento de chegada de um pacote em um armazém.
 *
 * Esta função processa a chegada de um pacote, atualizando seu estado e posição
 * conforme sua rota. Caso o pacote tenha chegado ao destino final, seu estado é
 * definido como ENTREGUE e uma mensagem é exibida. Caso contrário, o pacote é
 * armazenado no próximo armazém e seção de acordo com sua rota, e seu estado é
 * atualizado para POSTADO.
 *
 * @param evento Objeto do tipo Evento que representa o evento de chegada do pacote.
 * @param armazens Vetor de armazéns onde os pacotes podem ser armazenados.
 * @param pacotes Vetor contendo todos os pacotes em trânsito.
 */
void handleChegadaPacote(
    Evento evento, Vetor<Armazem>& armazens, 
    Vetor<Pacote<int>>& pacotes) {

    // Extrai informações do evento
    int idPacote = evento.getIdPacote();
    int tempoAtual = evento.getTempo();
    int armazemAntigo = pacotes[idPacote].getIdArmazemAtual() - 1; // Armazém atual do pacote (ajustado para índice 0)

    // Verifica se o pacote existe
    try {
        pacotes[idPacote];
    } catch (const std::out_of_range& e) {
        std::cerr << "Erro: Pacote com ID " << idPacote << " não encontrado." << std::endl;
        return;
    }

    /*
    * Verifica se o pacote chegou ao armazém final da rota.
    * Se sim, atualiza o estado do pacote para ENTREGUE e indica no log do sistema.
    */
    if (pacotes[idPacote].getIdArmazemAtual() == pacotes[idPacote].getRota().GetElemPos(pacotes[idPacote].getRota().GetTam())->GetData()) {
        pacotes[idPacote].setEstado(EstadoPacote::ENTREGUE);

        metricas.addDeliveryTime(tempoAtual - pacotes[idPacote].getTempoPostagem());

        std::cout << std::setfill('0')
                  << std::setw(7) << tempoAtual << " pacote "
                  << std::setw(3) << idPacote << " entregue em "
                  << std::setw(3) << armazemAntigo << std::endl;

        return;

    /*
    * Caso contrário, remove o armazém atual da rota,
    * atualiza o armazém e seção atuais do pacote, e o armazena no armazém correspondente.
    */ 
    } else {
        int armazemAtual = pacotes[idPacote].removeArmazemAtualDaRota();
        int secaoAtual = pacotes[idPacote].getProximoArmazemRota();
        
        pacotes[idPacote].setIdArmazemAtual(armazemAtual);
        pacotes[idPacote].setIdSecaoAtual(secaoAtual);
        pacotes[idPacote].setEstado(EstadoPacote::POSTADO);
        pacotes[idPacote].setUltimoTempoArmazenamento(tempoAtual);

        armazens[armazemAtual].armazenaPacote(secaoAtual, idPacote, metricas);

        metricas.addStorageTime(tempoAtual - pacotes[idPacote].getTempoPostagem());

        std::cout << std::setfill('0') 
                << std::setw(7) << tempoAtual << " pacote "
                << std::setw(3) << idPacote << " armazenado em "
                << std::setw(3) << armazemAtual - 1 << " na secao "
                << std::setw(3) << secaoAtual - 1
                << std::endl;
    }
}

/**
 * @brief Manipula o evento de transporte de pacotes entre armazéns.
 *
 * Esta função processa o transporte de pacotes de um armazém de origem para um armazém de destino,
 * atualizando o estado dos pacotes, agendando eventos de chegada e transporte futuros, e realizando
 * a rearmazenagem dos pacotes conforme necessário.
 *
 * @param evento Evento de transporte a ser processado, contendo informações sobre armazéns e tempo.
 * @param Escalonador Referência ao escalonador responsável pelo agendamento dos próximos eventos.
 * @param armazens Vetor contendo todos os armazéns do sistema.
 * @param pacotes Vetor contendo todos os pacotes do sistema.
 * @param custos Vetor com os custos associados às operações (latência, remoção, etc).
 */
void handleTransporte(
    Evento evento, Escalonador& Escalonador,
    Vetor<Armazem>& armazens, Vetor<Pacote<int>>& pacotes, Vetor<int>& custos) {

    // Extrai os armazéns envolvidos no evento de transporte
    Vetor<int> armazensEvento = evento.getArmazens();

    // Muda o relógio do sistema para o tempo do evento
    int tempoAtual = evento.getTempo();
    int latencia = custos[static_cast<int>(CustoEvento::LATENCIA_TRANSPORTE)];
    int remocao = custos[static_cast<int>(CustoEvento::CUSTO_REMOVER_PACOTE)];

    Armazem& armazemOrigem = armazens[armazensEvento[0]];

    /*
    * Adiciona os pacotes do armazém de origem para transporte para o armazém de destino.
    * Os pacotes restantes que não puderem ser transportados são armazenados em uma pilha para rearmazenamento.
    */
    Pilha<int> pacotesRearmazenar = armazemOrigem.adicionaPacotesParaTransporte(armazensEvento[1], tempoAtual, remocao, metricas);
    Lista<int> pacotesParaTransportar = armazemOrigem.getTransportesPorVizinho(armazensEvento[1]);

    if (pacotesParaTransportar.GetTam() > 0) {
        for (int idx = 1; idx <= pacotesParaTransportar.GetTam(); ++idx) {

            // Remove o pacote da lista de transporte e atualiza seu estado
            int idPacote = pacotesParaTransportar.GetElemPos(idx)->GetData();
            pacotes[idPacote].setEstado(EstadoPacote::EM_TRANSPORTE);
            pacotes[idPacote].setIdArmazemAtual(armazensEvento[1]);

            metricas.addTransitTime(tempoAtual - pacotes[idPacote].getUltimoTempoArmazenamento());
            metricas.incPackagesMoved();

            // Cria um evento de chegada do pacote no armazém de destino e o insere no escalonador
            Evento chegadaEvento(
                tempoAtual + latencia,
                idPacote,
                armazensEvento[1] - 1,
                pacotes[idPacote].getIdSecaoAtual(),
                TipoEvento::CHEGADA_PACOTE
            );
            Escalonador.InsereEvento(chegadaEvento, metricas);

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

    // Cria um novo evento de transporte para o próximo intervalo
    Evento proximoTransporte(
        tempoAtual + custos[2], // intervaloTransportes
        -1,
        armazensEvento[0] - 1,
        armazensEvento[1] - 1,
        TipoEvento::TRANSPORTE
    );
    Escalonador.InsereEvento(proximoTransporte, metricas);

    // Rearmazenar pacotes após o transporte
    armazemOrigem.rearmazenarPacotes(
        armazensEvento[1], pacotesRearmazenar, tempoAtual + timeDiff, metricas);
}

/**
 * @brief Função principal do programa.
 *
 * Esta função inicializa os principais componentes do sistema de entrega de pacotes,
 * incluindo o escalonador de eventos, as rotas de transporte, armazéns, pacotes e custos.
 * 
 * Após a leitura do arquivo, o escalonador é inicializado e o loop principal processa os eventos
 * até que todos os pacotes sejam entregues ou não haja mais eventos a serem processados.
 *
 * @param argc Número de argumentos da linha de comando.
 * @param argv Vetor de argumentos da linha de comando.
 * @return int Código de retorno da aplicação (0 para sucesso, 1 para erro de argumento).
 */
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

    metricas.updatePeakMemory();
    size_t initial_memory = metricas.getCurrentMemory();
    std::cout << "Memória inicial: " << initial_memory << " KB" << std::endl;

    leArquivo(nomeArquivo, rotas, armazens, escalonador, pacotes, custos);

    metricas.updatePeakMemory();
    size_t after_load_memory = metricas.getCurrentMemory();
    std::cout << "Memória depois de ler arquivo: " << after_load_memory 
            << " KB (+" << (after_load_memory - initial_memory) 
            << " KB)" << std::endl;
    
    // Inicializa as variáveis de controle do escalonador
    metricas.setTransportCapacity(custos[0]);
    metricas.startTimer();
    metricas.updatePeakMemory();

    while (!escalonador.Vazio()) {
        // Retira o próximo evento do escalonador
        metricas.updatePeakMemory();
        Evento prox_evento = escalonador.RetiraProximoEvento(metricas);

        metricas.updatePeakMemory();

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

        // Processa o evento de acordo com seu tipo
        switch (prox_evento.getTipoEvento())
        {
        case TipoEvento::CHEGADA_PACOTE:
            handleChegadaPacote(prox_evento, armazens, pacotes);
            metricas.updatePeakMemory();
            break;
        case TipoEvento::TRANSPORTE:
            handleTransporte(prox_evento, escalonador, armazens, pacotes, custos);
            metricas.incTransportEvents();
            metricas.updatePeakMemory();
            break;
        default:
            break;
        }
    }
    metricas.stopTimer();
    metricas.updatePeakMemory();

    size_t final_memory = metricas.getCurrentMemory();
    std::cout << "Memória final: " << final_memory << " KB, Pico: " 
            << metricas.getPeakMemory() << " KB" << std::endl;

    while (!escalonador.Vazio()) {
        metricas.updatePeakMemory();
        Evento prox_evento = escalonador.RetiraProximoEvento(metricas);
        metricas.decHeapExtract();
        metricas.updatePeakMemory();

        switch (prox_evento.getTipoEvento())
        {
        case TipoEvento::CHEGADA_PACOTE:
            break;
        case TipoEvento::TRANSPORTE:
            metricas.decTransportEvents();
            break;
        default:
            break;
        }
    }
    metricas.printMetrics();

    return (0);
}