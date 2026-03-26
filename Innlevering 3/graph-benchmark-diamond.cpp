#include "Algorithms.h"
#include "Cli.h"

#include "AbstractGraph.h"
#include "Node.h"

#include <chrono>
#include <exception>
#include <iostream>
#include <utility>
#include <vector>

int main(int argc, char** argv) {
    try {
        const DiamondCliOptions options = parse_diamond_cli(argc, argv);
        const std::vector<std::vector<std::string>> sequences = parse_query_file(options.query_file);

        const auto load_start = std::chrono::steady_clock::now();
        std::unique_ptr<AbstractGraph> graph = create_graph(options.representation);
        graph->load(options.graph_file);
        const auto load_end = std::chrono::steady_clock::now();

        const auto algorithm_start = std::chrono::steady_clock::now();
        const std::vector<std::pair<const Node*, const Node*>> pairs =
            find_diamond_pairs(*graph, sequences[0], sequences[1]);
        const auto algorithm_end = std::chrono::steady_clock::now();

        const auto load_ms = std::chrono::duration_cast<std::chrono::milliseconds>(load_end - load_start).count();
        const auto algorithm_ms =
            std::chrono::duration_cast<std::chrono::milliseconds>(algorithm_end - algorithm_start).count();

        if (options.output_mode == OutputMode::Silent) {
            std::cout << "representation=" << to_string(options.representation)
                      << " loaded_nodes=" << graph->nodes().size()
                      << " loaded_edges=" << graph->edges().size()
                      << " pairs=" << pairs.size()
                      << " load_ms=" << load_ms
                      << " diamond_ms=" << algorithm_ms << '\n';
            return 0;
        }

        std::cout << "Problem 3.2 - Diamond-Shaped Path Query\n";
        std::cout << "Representation: " << to_string(options.representation) << '\n';
        std::cout << "Output mode: " << to_string(options.output_mode) << '\n';
        if (options.counts_provided) {
            std::cout << "Expected nodes: " << options.expected_nodes << '\n';
            std::cout << "Expected edges: " << options.expected_edges << '\n';
        } else {
            std::cout << "Expected nodes: auto\n";
            std::cout << "Expected edges: auto\n";
        }
        std::cout << "Loaded nodes: " << graph->nodes().size() << '\n';
        std::cout << "Loaded edges: " << graph->edges().size() << '\n';
        std::cout << "Load time (ms): " << load_ms << '\n';
        std::cout << "Query time (ms): " << algorithm_ms << '\n';
        std::cout << "Sequence 1:";
        for (const std::string& label : sequences[0]) {
            std::cout << ' ' << label;
        }
        std::cout << '\n';
        std::cout << "Sequence 2:";
        for (const std::string& label : sequences[1]) {
            std::cout << ' ' << label;
        }
        std::cout << '\n';
        std::cout << "Matching node pairs: " << pairs.size() << "\n\n";

        for (const auto& pair : pairs) {
            std::cout << pair.first->label() << ' ' << pair.second->label() << '\n';
        }

        return 0;
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return 1;
    }
}
