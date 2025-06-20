#ifndef UTILS_HPP
#define UTILS_HPP

#include <stdexcept>
#include <iomanip>
#include <sstream>
#include <string>
#include "vetor.hpp"
#include "lista.hpp"

// ===== EVENTO CLASS =====
enum class TipoEvento {
    CHEGADA_PACOTE = 1,
    TRANSPORTE = 2
};

enum class CustoEvento {
    CAPACIDADE_TRANSPORTE,
    LATENCIA_TRANSPORTE,
    INTERVALO_TRANSPORTE,
    CUSTO_REMOVER_PACOTE,
};

class Evento {
    private:
        std::string _chave;
        int _tempo;

    public:
        Evento();
        Evento(std::string data);
        Evento(int tempo, int idPacote, int idArmazemOrigem, int idArmazemDestino, TipoEvento tipoEvento);

        std::string getData() const;
        int getTempo() const;
        int getIdPacote() const;
        Vetor<int> getArmazens() const;
        TipoEvento getTipoEvento() const;

        bool operator<(const Evento& other) const;
        bool operator>(const Evento& other) const;
        bool operator==(const Evento& other) const;
        bool operator!=(const Evento& other) const;
        bool operator<=(const Evento& other) const;
        bool operator>=(const Evento& other) const;
};

// ===== METRICAS CLASS =====
class Metricas {
    private:
        Lista<double> _tempos;
        Lista<double> _distancias;
        double _tempo_total;
        double _distancia_total;
    public:
        Metricas();

        void adicionaTempo(double tempo);
        void adicionaDistancia(double distancia);

        double getTempoTotal() const;
        double getDistanciaTotal() const;

        Lista<double> getTempos() const;
        Lista<double> getDistancias() const;
};

// ===== EVENTO IMPLEMENTATIONS =====


/**
 * @brief Construtor padrão da classe Evento.
 *
 * Inicializa um objeto Evento com chave vazia e tempo igual a -1.
 */
Evento::Evento()
    : _chave(""), _tempo(-1) {}


/**
 * @brief Construtor da classe Evento.
 *
 * Inicializa um objeto Evento a partir de uma chave fornecida.
 * A chave deve conter exatamente 13 caracteres, caso contrário,
 * uma exceção std::invalid_argument será lançada.
 * Os primeiros 6 caracteres da chave são convertidos para inteiro
 * e armazenados como o tempo do evento.
 *
 * @param chave String contendo a chave do evento (deve ter 13 caracteres).
 * @throw std::invalid_argument Se a chave não possuir exatamente 13 caracteres.
 */
Evento::Evento(std::string chave)
    : _chave(chave) {
    
    if (chave.length() != 13) {
        throw std::invalid_argument("Chave deve ter pelo menos 13 caracteres.");
    }

    this->_tempo = std::stoi(chave.substr(0, 6));
}

/**
 * @brief Construtor da classe Evento.
 *
 * Inicializa um objeto Evento com os parâmetros fornecidos.
 * O tempo não pode ser negativo, e o tipo de evento deve ser válido.
 * Dependendo do tipo de evento, a chave é formatada de maneira diferente.
 *
 * @param tempo Tempo do evento (não pode ser negativo).
 * @param idPacote ID do pacote associado ao evento (deve ser >= 0).
 * @param idArmazemOrigem ID do armazém de origem (deve ser >= 0 se idArmazemDestino também for >= 0).
 * @param idArmazemDestino ID do armazém de destino (deve ser >= 0 se idArmazemOrigem também for >= 0).
 * @param tipoEvento Tipo do evento (CHEGADA_PACOTE ou TRANSPORTE).
 * @throw std::invalid_argument Se o tempo for negativo ou se o tipo de evento for inválido.
 */
Evento::Evento(int tempo, int idPacote, int idArmazemOrigem, int idArmazemDestino, TipoEvento tipoEvento) 
    : _tempo(tempo) {
        
    int tipo = 0;

    if (tempo < 0) {
        throw std::invalid_argument("Tempo não pode ser negativo.");
    }

    switch (tipoEvento) {
        case TipoEvento::CHEGADA_PACOTE: tipo = 1; break;
        case TipoEvento::TRANSPORTE: tipo = 2; break;

        default:
            throw std::invalid_argument("Tipo de evento inválido.");
    }

    std::ostringstream oss;
    oss << std::setfill('0');

    // Sempre começa com o tempo (6 dígitos)
    oss << std::setw(6) << tempo;

    if (idPacote >= 0) {
        // ID de pacote válido: tempo(6) + pacote(6) + tipo(1)
        oss << std::setw(6) << idPacote;
        oss << tipo;
    } else if (idArmazemOrigem >= 0 && idArmazemDestino >= 0) {
        // Pacote inválido mas armazéns válidos: tempo(6) + origem(3) + destino(3) + tipo(1)
        oss << std::setw(3) << idArmazemOrigem;
        oss << std::setw(3) << idArmazemDestino;
        oss << tipo;
    } else {
        throw std::invalid_argument("Evento inválido: deve ter pacote válido ou armazéns válidos.");
    }

    this->_chave = oss.str();
}

/**
 * @brief Obtém a data do evento.
 *
 * Retorna a chave do evento, que representa a data e hora do evento.
 *
 * @return std::string A chave do evento.
 */
std::string Evento::getData() const {
    return this->_chave;
}

/**
 * @brief Obtém o tempo do evento.
 *
 * Retorna o tempo associado ao evento, que é o primeiro componente da chave.
 *
 * @return int O tempo do evento.
 */
int Evento::getTempo() const {
    return this->_tempo;
}

/**
 * @brief Obtém o ID do pacote associado ao evento.
 *
 * @return int ID do pacote associado ao evento.
 */
int Evento::getIdPacote() const {
    // O idPacote só está presente na chave se o evento for CHEGADA_PACOTE
    if (this->getTipoEvento() != TipoEvento::CHEGADA_PACOTE) {
        throw std::logic_error("Evento não possui idPacote.");
    }
    return std::stoi(this->_chave.substr(6, 6));
}

/**
 * @brief Obtém os IDs dos armazéns de origem e destino do evento.
 *
 * @return Vetor<int> Vetor contendo os IDs dos armazéns de origem e destino.
 * @throws std::logic_error Se o evento não for do tipo TRANSPORTE.
 */
Vetor<int> Evento::getArmazens() const {
    // Apenas TRANSPORTE possui armazemOrigem e armazemDestino
    TipoEvento tipo = this->getTipoEvento();
    if (tipo != TipoEvento::TRANSPORTE) {
        throw std::logic_error("Evento não possui armazéns de origem e destino.");
    }

    Vetor<int> armazens(2);
    armazens[0] = std::stoi(this->_chave.substr(6, 3)) + 1; // armazemOrigem
    armazens[1] = std::stoi(this->_chave.substr(9, 3)) + 1; // armazemDestino
    return armazens;
}

/**
 * @brief Obtém o tipo do evento.
 *
 * Retorna o tipo do evento, que é determinado pelo último caractere da chave.
 *
 * @return TipoEvento O tipo do evento (CHEGADA_PACOTE ou TRANSPORTE).
 * @throws std::logic_error Se o tipo de evento for desconhecido.
 */
TipoEvento Evento::getTipoEvento() const {
    // tipoEvento sempre é o último caractere da chave
    int tipo = std::stoi(this->_chave.substr(this->_chave.length() - 1, 1));

    switch (tipo) {
        case 1: return TipoEvento::CHEGADA_PACOTE;
        case 2: return TipoEvento::TRANSPORTE;
        default: throw std::logic_error("Tipo de evento desconhecido.");
    }
}

// ===== OPERATOR OVERLOADS IMPLEMENTATIONS =====
bool Evento::operator<(const Evento& other) const {
    return this->_chave < other._chave;
}

bool Evento::operator>(const Evento& other) const {
    return this->_chave > other._chave;
}

bool Evento::operator==(const Evento& other) const {
    return this->_chave == other._chave;
}

bool Evento::operator!=(const Evento& other) const {
    return this->_chave != other._chave;
}

bool Evento::operator<=(const Evento& other) const {
    return this->_chave <= other._chave;
}

bool Evento::operator>=(const Evento& other) const {
    return this->_chave >= other._chave;
}

// ===== METRICAS IMPLEMENTATIONS =====

/**
 * @brief Construtor padrão da classe Metricas.
 *
 * Inicializa os atributos de tempo total e distância total como zero,
 * e cria listas vazias para armazenar os tempos e distâncias individuais.
 */
Metricas::Metricas() : _tempo_total(0), _distancia_total(0) {}

/**
 * @brief Adiciona um tempo à lista de tempos e atualiza o tempo total.
 *
 * Insere o tempo fornecido no final da lista de tempos e acumula
 * o valor no tempo total.
 *
 * @param tempo Tempo a ser adicionado (em segundos).
 */
void Metricas::adicionaTempo(double tempo) {
    this->_tempos.InsereFim(tempo);
    this->_tempo_total += tempo;
}

/**
 * @brief Adiciona uma distância à lista de distâncias e atualiza a distância total.
 *
 * Insere a distância fornecida no final da lista de distâncias e acumula
 * o valor na distância total.
 *
 * @param distancia Distância a ser adicionada (em metros).
 */
void Metricas::adicionaDistancia(double distancia) {
    this->_distancias.InsereFim(distancia);
    this->_distancia_total += distancia;
}

/**
 * @brief Obtém o tempo total acumulado.
 *
 * Retorna o valor total de tempo acumulado em segundos.
 *
 * @return double Tempo total (em segundos).
 */
double Metricas::getTempoTotal() const {
    return this->_tempo_total;
}

/**
 * @brief Obtém a distância total acumulada.
 *
 * Retorna o valor total de distância acumulada em metros.
 *
 * @return double Distância total (em metros).
 */
double Metricas::getDistanciaTotal() const {
    return this->_distancia_total;
}

/**
 * @brief Obtém a lista de tempos individuais.
 *
 * Retorna uma lista contendo todos os tempos registrados.
 *
 * @return Lista<double> Lista de tempos (em segundos).
 */
Lista<double> Metricas::getTempos() const {
    return this->_tempos;
}

/**
 * @brief Obtém a lista de distâncias individuais.
 *
 * Retorna uma lista contendo todas as distâncias registradas.
 *
 * @return Lista<double> Lista de distâncias (em metros).
 */
Lista<double> Metricas::getDistancias() const {
    return this->_distancias;
}

#endif
