#include "Algorithms.h"
#include "Cli.h"

#include "AbstractGraph.h"
#include "Node.h"

#include <chrono>
#include <exception>
#include <iostream>
#include <vector>

int main(int argc, char** argv) {
    try {
        const CommonCliOptions options = parse_scc_cli(argc, argv);

        const auto load_start = std::chrono::steady_clock::now();
        std::unique_ptr<AbstractGraph> graph = create_graph(options.representation);
        graph->load(options.graph_file);
        const auto load_end = std::chrono::steady_clock::now();

        const auto algorithm_start = std::chrono::steady_clock::now();
        const std::vector<std::vector<const Node*>> components =
            tarjan_strongly_connected_components(*graph);
        const auto algorithm_end = std::chrono::steady_clock::now();

        const auto load_ms = std::chrono::duration_cast<std::chrono::milliseconds>(load_end - load_start).count();
        const auto algorithm_ms =
            std::chrono::duration_cast<std::chrono::milliseconds>(algorithm_end - algorithm_start).count();

        if (options.output_mode == OutputMode::Silent) {
            std::cout << "representation=" << to_string(options.representation)
                      << " loaded_nodes=" << graph->nodes().size()
                      << " loaded_edges=" << graph->edges().size()
                      << " components=" << components.size()
                      << " load_ms=" << load_ms
                      << " scc_ms=" << algorithm_ms << '\n';
            return 0;
        }

        std::cout << "Problem 3.1 - Strongly Connected Components\n";
        std::cout << "Representation: " << to_string(options.representation) << '\n';
        std::cout << "Output mode: " << to_string(options.output_mode) << '\n';
        std::cout << "Expected nodes: " << options.expected_nodes << '\n';
        std::cout << "Expected edges: " << options.expected_edges << '\n';
        std::cout << "Loaded nodes: " << graph->nodes().size() << '\n';
        std::cout << "Loaded edges: " << graph->edges().size() << '\n';
        std::cout << "Load time (ms): " << load_ms << '\n';
        std::cout << "Tarjan time (ms): " << algorithm_ms << '\n';
        std::cout << "Strongly connected components: " << components.size() << "\n\n";

        for (std::size_t index = 0; index < components.size(); ++index) {
            std::cout << "Component " << (index + 1) << " (size " << components[index].size() << "):";
            for (const Node* node : components[index]) {
                std::cout << ' ' << node->label();
            }
            std::cout << '\n';
        }

        return 0;
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return 1;
    }
}
