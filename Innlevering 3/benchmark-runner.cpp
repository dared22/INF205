#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <string>


int main() {

    system("mkdir data 2>nul");
    system("mkdir results 2>nul");
    
    std::cout << "\nTask 3.4 - Scaling & Runtime Measurement\n\n";
    
    std::vector<int> sizes = {100, 500, 1000, 5000, 10000};

    std::ofstream csv("results/benchmark_results.csv");
    csv << "n,representation,time_ms\n";
    

    std::cout << "1. DIAMOND PATH - Constant Query Size (m=5)\n\n";
    
    for (int size : sizes) {
        std::cout << "  n=" << size << ":\n";

        std::string gen_cmd = "generate-graph " + std::to_string(size) + " 5 " +
                             "data/graph_" + std::to_string(size) + ".dat " +
                             "data/query_" + std::to_string(size) + ".dat > nul";
        system(gen_cmd.c_str());
        
        // Benchmark LINKED representation
        std::string linked_cmd = std::string("graph-benchmark-diamond ") +
                                "data/graph_" + std::to_string(size) + ".dat " +
                                "data/query_" + std::to_string(size) + ".dat " +
                                "--linked -s";
        
        auto start = std::chrono::high_resolution_clock::now();
        system(linked_cmd.c_str());
        auto end = std::chrono::high_resolution_clock::now();
        
        long long ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "    linked:  " << ms << " ms\n";
        csv << size << ",linked," << ms << "\n";
        
        
        // Benchmark MATRIX representation
        std::string matrix_cmd = std::string("graph-benchmark-diamond ") +
                                "data/graph_" + std::to_string(size) + ".dat " +
                                "data/query_" + std::to_string(size) + ".dat " +
                                "--matrix -s";
        
        start = std::chrono::high_resolution_clock::now();
        system(matrix_cmd.c_str());
        end = std::chrono::high_resolution_clock::now();
        
        ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "    matrix:  " << ms << " ms\n\n";
        csv << size << ",matrix," << ms << "\n";
    }
    
    
// diamond path with proportional query size (m=n/10)
    std::cout << "2. DIAMOND PATH - Proportional Query Size (m=n/10)\n\n";

    for (int size : sizes) {
        int qsize = size / 10;
        if (qsize < 1) qsize = 1;
        
        std::cout << "  n=" << size << " (m=" << qsize << "):\n";
        
        // generate
        std::string gen_cmd = "generate-graph " + std::to_string(size) + " " + 
                             std::to_string(qsize) + " " +
                             "data/graph_prop_" + std::to_string(size) + ".dat " +
                             "data/query_prop_" + std::to_string(size) + ".dat > nul";
        system(gen_cmd.c_str());
        
        // linked representation
        std::string linked_cmd = std::string("graph-benchmark-diamond ") +
                                "data/graph_prop_" + std::to_string(size) + ".dat " +
                                "data/query_prop_" + std::to_string(size) + ".dat " +
                                "--linked -s";
        
        auto start = std::chrono::high_resolution_clock::now();
        system(linked_cmd.c_str());
        auto end = std::chrono::high_resolution_clock::now();
        
        long long ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "    linked:  " << ms << " ms\n";
        csv << size << ",linked," << ms << "\n";
        
        // matrix representation
        std::string matrix_cmd = std::string("graph-benchmark-diamond ") +
                                "data/graph_prop_" + std::to_string(size) + ".dat " +
                                "data/query_prop_" + std::to_string(size) + ".dat " +
                                "--matrix -s";
        
        start = std::chrono::high_resolution_clock::now();
        system(matrix_cmd.c_str());
        end = std::chrono::high_resolution_clock::now();
        
        ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "    matrix:  " << ms << " ms\n\n";
        csv << size << ",matrix," << ms << "\n";
    }
    
    
// SCC algorithm
    std::cout << "3. SCC ALGORITHM - Tarjan's Algorithm\n\n";
    
    for (int size : sizes) {
        int num_edges = size * 10;
        
        std::cout << "  n=" << size << " (e=" << num_edges << "):\n";

        // generate 
        std::string gen_cmd = "generate-scc " + std::to_string(size) + " " +
                             std::to_string(num_edges) + " " +
                             "data/scc_graph_" + std::to_string(size) + ".dat > nul";
        system(gen_cmd.c_str());
        
        // linked representation
        std::string linked_cmd = std::string("graph-benchmark-scc ") +
                                "data/scc_graph_" + std::to_string(size) + ".dat " +
                                "--linked -s";
        
        auto start = std::chrono::high_resolution_clock::now();
        system(linked_cmd.c_str());
        auto end = std::chrono::high_resolution_clock::now();
        
        long long ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "    linked:  " << ms << " ms\n";
        csv << size << ",linked," << ms << "\n";
        
        // matrix representation
        std::string matrix_cmd = std::string("graph-benchmark-scc ") +
                                "data/scc_graph_" + std::to_string(size) + ".dat " +
                                "--matrix -s";
        
        start = std::chrono::high_resolution_clock::now();
        system(matrix_cmd.c_str());
        end = std::chrono::high_resolution_clock::now();
        
        ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "    matrix:  " << ms << " ms\n\n";
        csv << size << ",matrix," << ms << "\n";
    }
    
    
    csv.close();
    
    std::cout << "\nBenchmark Complete\n";
    std::cout << "Results saved to: results/benchmark_results.csv\n";
    std::cout << "Data files saved to: data/\n";
    std::cout << "\nTo generate plots, run: python plot_benchmarks.py\n\n";
    
    return 0;
}
