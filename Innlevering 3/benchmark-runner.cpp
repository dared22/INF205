/*
 * Simple C++ benchmark runner for task 3.4
 * Outputs results directly to console (no CSV file)
 */

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

// Run command and capture output
std::string run_command(const std::string& cmd) {
    std::string temp_file = "_temp_out.txt";
    std::string full_cmd = cmd + " > " + temp_file + " 2>&1";
    
    int ret = system(full_cmd.c_str());
    if (ret != 0) {
        return "";
    }
    
    std::ifstream file(temp_file);
    std::string result;
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            result += line + "\n";
        }
        file.close();
        remove(temp_file.c_str());
    }
    return result;
}

// Extract value from output like "diamond_ms=23"
int extract_value(const std::string& output, const std::string& key) {
    size_t pos = output.find(key + "=");
    if (pos != std::string::npos) {
        pos += key.length() + 1;
        size_t end = output.find(" ", pos);
        if (end == std::string::npos) {
            end = output.find("\n", pos);
        }
        if (end == std::string::npos) {
            end = output.length();
        }
        try {
            return std::stoi(output.substr(pos, end - pos));
        } catch (...) {
            return 0;
        }
    }
    return 0;
}

void run_diamond_benchmark(int size, int query_size, const std::string& rep, std::ofstream& csv) {
    std::string graph_file = "data/graph_" + std::to_string(size) + ".dat";
    std::string query_file = "data/query_" + std::to_string(size) + ".dat";
    
    // Generate graph
    std::string gen_cmd = "generate-graph " + std::to_string(size) + " " + 
                          std::to_string(query_size) + " " + graph_file + " " + query_file + " > nul";
    system(gen_cmd.c_str());
    
    // Run benchmark
    std::string bench_cmd = "graph-benchmark-diamond " + graph_file + " " + query_file + 
                           " --" + rep + " -s";
    std::string output = run_command(bench_cmd);
    
    // Extract and display results
    int nodes = extract_value(output, "loaded_nodes");
    int edges = extract_value(output, "loaded_edges");
    int time_ms = extract_value(output, "diamond_ms");
    
    std::cout << "    " << std::setw(6) << std::left << rep 
              << " | nodes: " << std::setw(5) << nodes
              << " | edges: " << std::setw(6) << edges
              << " | time: " << std::setw(4) << time_ms << " ms\n";
    
    // Write to CSV
    csv << size << "," << rep << "," << time_ms << "\n";
}

void run_scc_benchmark(int size, int num_edges, const std::string& rep, std::ofstream& csv) {
    std::string graph_file = "data/scc_graph_" + std::to_string(size) + ".dat";
    
    // Generate graph
    std::string gen_cmd = "generate-scc " + std::to_string(size) + " " + 
                          std::to_string(num_edges) + " " + graph_file + " > nul";
    system(gen_cmd.c_str());
    
    // Run benchmark
    std::string bench_cmd = "graph-benchmark-scc " + graph_file + " --" + rep + " -s";
    std::string output = run_command(bench_cmd);
    
    // Extract and display results
    int nodes = extract_value(output, "loaded_nodes");
    int edges = extract_value(output, "loaded_edges");
    int time_ms = extract_value(output, "scc_ms");
    
    std::cout << "    " << std::setw(6) << std::left << rep 
              << " | nodes: " << std::setw(5) << nodes
              << " | edges: " << std::setw(6) << edges
              << " | time: " << std::setw(4) << time_ms << " ms\n";
    
    // Write to CSV
    csv << size << "," << rep << "," << time_ms << "\n";
}

int main() {
    // Create folders
    system("mkdir data 2>nul");
    system("mkdir results 2>nul");
    
    std::cout << "\n";
    std::cout << "================================================\n";
    std::cout << "     Task 3.4 - Scaling & Runtime Measurement\n";
    std::cout << "================================================\n";
    
    std::vector<int> sizes = {100, 500, 1000, 5000, 10000};
    
    // Open CSV file for data export
    std::ofstream csv("results/benchmark_results.csv");
    csv << "n,representation,time_ms\n";
    
    // DIAMOND PATH - Constant query size
    std::cout << "\n[1] DIAMOND PATH - Constant Query Size (m=5)\n";
    std::cout << "--------------------------------------------\n";
    
    for (int size : sizes) {
        std::cout << "  n=" << size << ":\n";
        run_diamond_benchmark(size, 5, "linked", csv);
        run_diamond_benchmark(size, 5, "matrix", csv);
    }
    
    csv << "\n";
    
    // DIAMOND PATH - Proportional query size
    std::cout << "\n[2] DIAMOND PATH - Proportional Query Size (m=n/10)\n";
    std::cout << "---------------------------------------------------\n";
    
    for (int size : sizes) {
        int qsize = size / 10;
        if (qsize < 1) qsize = 1;
        
        std::cout << "  n=" << size << " (m=" << qsize << "):\n";
        run_diamond_benchmark(size, qsize, "linked", csv);
        run_diamond_benchmark(size, qsize, "matrix", csv);
    }
    
    csv << "\n";
    
    // SCC ALGORITHM
    std::cout << "\n[3] SCC ALGORITHM - Tarjan's Algorithm\n";
    std::cout << "--------------------------------------\n";
    
    for (int size : sizes) {
        int num_edges = size * 10;
        
        std::cout << "  n=" << size << " (e=" << num_edges << "):\n";
        run_scc_benchmark(size, num_edges, "linked", csv);
        run_scc_benchmark(size, num_edges, "matrix", csv);
    }
    
    csv.close();
    
    std::cout << "\n================================================\n";
    std::cout << "  Benchmark Complete!\n";
    std::cout << "================================================\n\n";
    
    std::cout << "Results saved to: results/benchmark_results.csv\n";
    std::cout << "Data files saved to: data/\n";
    std::cout << "\nTo generate plots, run: python plot_benchmarks.py\n";
    std::cout << "\n";
    
    return 0;
}
