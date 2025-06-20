#ifndef METRICAS_HPP
#define METRICAS_HPP

#include "lista.hpp"
#include <vector>
#include <chrono>
#include <string>

class Metricas {
    private:
        Lista<int> _tempos;
        Lista<int> _distancias;
        int _tempo_total;
        int _distancia_total;
        // Timing
        std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
        std::chrono::time_point<std::chrono::high_resolution_clock> end_time;
        // Memory
        size_t peak_memory;
        // Heap operations
        size_t heap_inserts;
        size_t heap_extracts;
        // Stack operations
        size_t stack_pushes;
        size_t stack_pops;
        size_t stack_pops_rearmazenado;
        // Re-storage events
        size_t re_storage_events;
        // Warehouse section depth
        size_t max_section_depth;
        size_t total_section_depth;
        size_t section_depth_samples;
        // Transport utilization
        size_t packages_moved;
        size_t transport_capacity;
        size_t transport_events;
        // Package delivery times
        std::vector<int> delivery_times;
        std::vector<int> transit_times;
        std::vector<int> storage_times;
    public:
        Metricas();
        // Timing
        void startTimer();
        void stopTimer();
        double getTotalExecutionTime() const;        // Memory
        void updatePeakMemory();
        size_t getPeakMemory() const;
        size_t getCurrentMemory() const;
        // Heap operations
        void incHeapInsert();
        void incHeapExtract();
        void decHeapExtract();
        // Stack operations
        void incStackPush();
        void incStackPop(bool re_storage = false);
        // Re-storage events
        void incReStorage();
        // Warehouse section depth
        void sampleSectionDepth(size_t depth);
        double getAvgSectionDepth() const;
        size_t getMaxSectionDepth() const;
        // Transport utilization
        void incPackagesMoved();
        void setTransportCapacity(size_t cap);
        void incTransportEvents();
        void decTransportEvents();
        double getTransportUtilization() const;
        // Package times
        void addDeliveryTime(int t);
        void addTransitTime(int t);
        void addStorageTime(int t);
        // Output all metrics
        void printMetrics(const std::string& filename = "/home/isaacreyes/Documents/ed/TP2-ED/metrics.json") const;

        void adicionaTempo(int tempo);

        void adicionaDistancia(int distancia);

        int getTempoTotal() const;

        int getDistanciaTotal() const;

        Lista<int> getTempos() const;

        Lista<int> getDistancias() const;

};

#endif