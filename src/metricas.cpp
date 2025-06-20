#include "metricas.hpp"
#include <fstream>
#include <algorithm>
#include <sys/resource.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include "json.hpp" // For JSON output, add to your project if not present

Metricas::Metricas() : _tempo_total(0), _distancia_total(0), peak_memory(0), heap_inserts(0), heap_extracts(0), stack_pushes(0), stack_pops(0), stack_pops_rearmazenado(0), re_storage_events(0), max_section_depth(0), total_section_depth(0), section_depth_samples(0), packages_moved(0), transport_capacity(0), transport_events(0) {}

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
    // Method 1: Try to read current RSS from /proc/self/status (more accurate)
    std::ifstream status_file("/proc/self/status");
    if (status_file.is_open()) {
        std::string line;
        while (std::getline(status_file, line)) {
            if (line.substr(0, 6) == "VmRSS:") {
                std::istringstream iss(line);
                std::string label, unit;
                size_t current_rss;
                iss >> label >> current_rss >> unit;
                
                if (current_rss > peak_memory) {
                    peak_memory = current_rss;
                }
                status_file.close();
                return;
            }
        }
        status_file.close();
    }
    
    // Method 2: Fallback to getrusage if /proc method fails
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        // On Linux, ru_maxrss is in kilobytes
        size_t current_memory = static_cast<size_t>(usage.ru_maxrss);
        if (current_memory > peak_memory) {
            peak_memory = current_memory;
        }
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
void Metricas::addDeliveryTime(int t) { delivery_times.push_back(t); }
void Metricas::addTransitTime(int t) { transit_times.push_back(t); }
void Metricas::addStorageTime(int t) { storage_times.push_back(t); }
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
    auto stats = [](const std::vector<int>& v) {
        nlohmann::json s;
        if (v.empty()) return s;
        std::vector<int> sorted = v;
        std::sort(sorted.begin(), sorted.end());
        s["mean"] = std::accumulate(sorted.begin(), sorted.end(), 0.0) / sorted.size();
        s["median"] = sorted[sorted.size()/2];
        s["p95"] = sorted[sorted.size()*95/100];
        s["max"] = sorted.back();
        return s;
    };
    j["delivery_time_stats"] = stats(delivery_times);
    j["transit_time_stats"] = stats(transit_times);
    j["storage_time_stats"] = stats(storage_times);
    std::ofstream f(filename);
    f << j.dump(2);

    std::cout << "Metrics saved to " << filename << std::endl;
}