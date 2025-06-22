#ifndef UTILS_HPP
#define UTILS_HPP

#include <stdexcept>
#include <iomanip>
#include <sstream>
#include <string>
#include <fstream>
#include <sys/resource.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <chrono>
#include <string>
#include "json.hpp"
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
        Lista<int> _tempos;
        Lista<int> _distancias;
        int _tempo_total;
        int _distancia_total;
        // Métricas de relógio
        std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
        std::chrono::time_point<std::chrono::high_resolution_clock> end_time;
        // Memória
        size_t peak_memory;
        // Operações de heap
        size_t heap_inserts;
        size_t heap_extracts;
        // Operações de pilha
        size_t stack_pushes;
        size_t stack_pops;
        size_t stack_pops_rearmazenado;
        // Operações de re-armazenamento
        size_t re_storage_events;
        // Profundidade das seções do armazém
        size_t max_section_depth;
        size_t total_section_depth;
        size_t section_depth_samples;
        // Utilização do transporte
        size_t packages_moved;
        size_t transport_capacity;
        size_t transport_events;
        // Tempo de entrega, trânsito e armazenamento dos pacotes
        Vetor<int> delivery_times;
        Vetor<int> transit_times;
        Vetor<int> storage_times;
        // Número de re-roteamentos dinâmicos
        size_t rerouting_count;
    public:
        Metricas();
        // Métricas de relógio
        void startTimer();
        void stopTimer();
        double getTotalExecutionTime() const;        
        // Memória
        void updatePeakMemory();
        size_t getPeakMemory() const;
        size_t getCurrentMemory() const;
        // Operações de heap
        void incHeapInsert();
        void incHeapExtract();
        void decHeapExtract();
        // Operações de pilha
        void incStackPush();
        void incStackPop(bool re_storage = false);
        // Operações de re-armazenamento
        void incReStorage();
        // Profundidade das seções do armazém
        void sampleSectionDepth(size_t depth);
        double getAvgSectionDepth() const;
        size_t getMaxSectionDepth() const;
        // Utilização do transporte
        void incPackagesMoved();
        void setTransportCapacity(size_t cap);
        void incTransportEvents();
        void decTransportEvents();
        double getTransportUtilization() const;
        // Tempo de entrega, trânsito e armazenamento dos pacotes
        void addDeliveryTime(int t);
        void addTransitTime(int t);
        void addStorageTime(int t);
        // Rer-roteamento dinâmico
        void incReroutingCount();
        size_t getReroutingCount() const;
        // Calcula e imprime as métricas em formato JSON
        void printMetrics(const std::string& filename = "metrics.json") const;

        void adicionaTempo(int tempo);

        void adicionaDistancia(int distancia);

        int getTempoTotal() const;

        int getDistanciaTotal() const;

        Lista<int> getTempos() const;

        Lista<int> getDistancias() const;

};

// Funções auxiliares para estatísticas e ordenação
void quickSort(Vetor<int>& arr, int low, int high);
int partition(Vetor<int>& arr, int low, int high);
double accumulate(const Vetor<int>& v);

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

Metricas::Metricas() : 
    _tempo_total(0), _distancia_total(0), peak_memory(0), heap_inserts(0), 
    heap_extracts(0), stack_pushes(0), stack_pops(0), stack_pops_rearmazenado(0), 
    re_storage_events(0), max_section_depth(0), total_section_depth(0), 
    section_depth_samples(0), packages_moved(0), transport_capacity(0), 
    transport_events(0), delivery_times(), transit_times(), storage_times(), 
    rerouting_count(0) {

    // Initialize vectors as empty by removing the default garbage element
    if (delivery_times.getSize() > 0) {
        delivery_times.remove(0);
    }
    if (transit_times.getSize() > 0) {
        transit_times.remove(0);
    }
    if (storage_times.getSize() > 0) {
        storage_times.remove(0);
    }
    
    // Initialize peak_memory with current memory usage
    updatePeakMemory();
}

// Timing
void Metricas::startTimer() {
    start_time = std::chrono::high_resolution_clock::now();
}
void Metricas::stopTimer() {
    end_time = std::chrono::high_resolution_clock::now();
}
double Metricas::getTotalExecutionTime() const {
    return std::chrono::duration<double>(end_time - start_time).count();
}
// Memory
void Metricas::updatePeakMemory() {
    size_t current_memory = 0;
    
    // Method 1: Try to read current RSS from /proc/self/status (more accurate)
    std::ifstream status_file("/proc/self/status");
    if (status_file.is_open()) {
        std::string line;
        while (std::getline(status_file, line)) {
            if (line.substr(0, 6) == "VmRSS:") {
                std::istringstream iss(line);
                std::string label, unit;
                iss >> label >> current_memory >> unit;
                status_file.close();
                break;
            }
        }
        status_file.close();
    }
    
    // Method 2: Fallback to getrusage if /proc method fails
    if (current_memory == 0) {
        struct rusage usage;
        if (getrusage(RUSAGE_SELF, &usage) == 0) {
            // On Linux, ru_maxrss is in kilobytes
            current_memory = static_cast<size_t>(usage.ru_maxrss);
        }
    }
    
    // Update peak if current is higher or if peak is not set yet
    if (current_memory > peak_memory || peak_memory == 0) {
        peak_memory = current_memory;
    }
}
size_t Metricas::getPeakMemory() const { return peak_memory; }

// Get current memory usage in KB
size_t Metricas::getCurrentMemory() const {
    std::ifstream status_file("/proc/self/status");
    if (status_file.is_open()) {
        std::string line;
        while (std::getline(status_file, line)) {
            if (line.substr(0, 6) == "VmRSS:") {
                std::istringstream iss(line);
                std::string label, unit;
                size_t current_rss;
                iss >> label >> current_rss >> unit;
                return current_rss;
            }
        }
    }
    
    // Fallback to getrusage
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        return static_cast<size_t>(usage.ru_maxrss);
    }
    
    return 0;
}
// Heap operations
void Metricas::incHeapInsert() { ++heap_inserts; }
void Metricas::incHeapExtract() { ++heap_extracts; }
void Metricas::decHeapExtract() {
    if (heap_extracts > 0) --heap_extracts; // Prevent negative counts
}
// Stack operations
void Metricas::incStackPush() { ++stack_pushes; }
void Metricas::incStackPop(bool re_storage) {
    ++stack_pops;
    if (re_storage) ++stack_pops_rearmazenado;
}
// Re-storage events
void Metricas::incReStorage() { ++re_storage_events; }
// Warehouse section depth
void Metricas::sampleSectionDepth(size_t depth) {
    if (depth > max_section_depth) max_section_depth = depth;
    total_section_depth += depth;
    ++section_depth_samples;
}
double Metricas::getAvgSectionDepth() const {
    return section_depth_samples ? (double)total_section_depth / section_depth_samples : 0.0;
}
size_t Metricas::getMaxSectionDepth() const { return max_section_depth; }
// Transport utilization
void Metricas::incPackagesMoved() { ++packages_moved; }
void Metricas::setTransportCapacity(size_t cap) { transport_capacity = cap; }
void Metricas::incTransportEvents() { ++transport_events; }
void Metricas::decTransportEvents() {
    if (transport_events > 0) --transport_events; // Prevent negative counts
}
double Metricas::getTransportUtilization() const {
    if (transport_capacity == 0 || transport_events == 0) return 0.0;
    return 100.0 * packages_moved / (transport_capacity * transport_events);
}
// Package times
void Metricas::addDeliveryTime(int t) { 
    delivery_times.insereFim(t); 
}
void Metricas::addTransitTime(int t) { 
    transit_times.insereFim(t); 
}
void Metricas::addStorageTime(int t) { 
    storage_times.insereFim(t); 
}

// Custom sorting function using quicksort
void quickSort(Vetor<int>& arr, int low, int high) {
    if (low < high) {
        // Partition the array
        int pivot = partition(arr, low, high);
        
        // Sort elements before and after partition
        quickSort(arr, low, pivot - 1);
        quickSort(arr, pivot + 1, high);
    }
}

int partition(Vetor<int>& arr, int low, int high) {
    int pivot = arr[high]; // Choose rightmost element as pivot
    int i = (low - 1); // Index of smaller element
    
    for (int j = low; j <= high - 1; j++) {
        // If current element is smaller than or equal to pivot
        if (arr[j] <= pivot) {
            i++; // increment index of smaller element
            // Swap elements
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    // Swap pivot with element at i+1
    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    return (i + 1);
}

// Custom accumulation function
double accumulate(const Vetor<int>& v) {
    double sum = 0.0;
    for (int i = 0; i < v.getSize(); i++) {
        sum += v[i];
    }
    return sum;
}

// Output all metrics
void Metricas::printMetrics(const std::string& filename) const {
    nlohmann::json j;
    j["execution_time"] = getTotalExecutionTime();
    j["peak_memory_kb"] = getPeakMemory();
    j["heap_inserts"] = heap_inserts;
    j["heap_extracts"] = heap_extracts;
    j["stack_pushes"] = stack_pushes;
    j["stack_pops"] = stack_pops;
    j["stack_pops_rearmazenado"] = stack_pops_rearmazenado;
    j["re_storage_events"] = re_storage_events;
    j["max_section_depth"] = getMaxSectionDepth();
    j["avg_section_depth"] = getAvgSectionDepth();
    j["packages_moved"] = packages_moved;
    j["transport_capacity"] = transport_capacity;
    j["transport_events"] = transport_events;
    j["transport_utilization"] = getTransportUtilization();
    j["rerouting_count"] = getReroutingCount();
    
    auto stats = [](const Vetor<int>& v) {
        nlohmann::json s;
        if (v.getSize() == 0) return s;
        
        // Create a copy for sorting - handle the default constructor issue
        Vetor<int> sorted;
        // Remove the garbage element from default constructor
        if (sorted.getSize() > 0) {
            sorted.remove(0);
        }
        
        // Now copy actual data
        for (int i = 0; i < v.getSize(); i++) {
            sorted.insereFim(v[i]);
        }
        
        // Sort the copy
        if (sorted.getSize() > 1) {
            quickSort(sorted, 0, sorted.getSize() - 1);
        }
        
        // Calculate statistics
        double sum = 0.0;
        for (int i = 0; i < sorted.getSize(); i++) {
            sum += sorted[i];
        }
        
        s["mean"] = sum / sorted.getSize();
        
        // Safe median calculation
        int medianIndex = sorted.getSize() / 2;
        if (sorted.getSize() % 2 == 0 && sorted.getSize() > 1) {
            // Even number of elements: average of two middle elements
            s["median"] = (sorted[medianIndex - 1] + sorted[medianIndex]) / 2.0;
        } else {
            // Odd number of elements: middle element
            s["median"] = sorted[medianIndex];
        }
        
        // Safe percentile calculation
        int p95Index = (sorted.getSize() * 95) / 100;
        if (p95Index >= sorted.getSize()) p95Index = sorted.getSize() - 1;
        s["p95"] = sorted[p95Index];
        
        s["max"] = sorted[sorted.getSize() - 1];
        s["min"] = sorted[0];
        s["count"] = sorted.getSize();
        return s;
    };
    
    j["delivery_time_stats"] = stats(delivery_times);
    j["transit_time_stats"] = stats(transit_times);
    j["storage_time_stats"] = stats(storage_times);
    std::ofstream f(filename);
    f << j.dump(2);

    std::cout << "Metrics saved to " << filename << std::endl;
}

void Metricas::adicionaTempo(int tempo) {
    _tempos.InsereFim(tempo);
    _tempo_total += tempo;
}

void Metricas::adicionaDistancia(int distancia) {
    _distancias.InsereFim(distancia);
    _distancia_total += distancia;
}

int Metricas::getTempoTotal() const {
    return _tempo_total;
}

int Metricas::getDistanciaTotal() const {
    return _distancia_total;
}

Lista<int> Metricas::getTempos() const {
    return _tempos;
}

Lista<int> Metricas::getDistancias() const {
    return _distancias;
}

// Dynamic routing metrics methods
void Metricas::incReroutingCount() {
    rerouting_count++;
}

size_t Metricas::getReroutingCount() const {
    return rerouting_count;
}

#endif
