#ifndef CLI_H
#define CLI_H

#include <memory>
#include <string>
#include <vector>

class AbstractGraph;

enum class OutputMode {
    Silent,
    Verbose
};

enum class GraphRepresentation {
    Linked,
    Matrix
};

struct CommonCliOptions {
    long expected_nodes = 0;
    long expected_edges = 0;
    std::string graph_file;
    OutputMode output_mode = OutputMode::Verbose;
    GraphRepresentation representation = GraphRepresentation::Linked;
};

struct DiamondCliOptions : CommonCliOptions {
    std::string query_file;
};

CommonCliOptions parse_scc_cli(int argc, char** argv);
DiamondCliOptions parse_diamond_cli(int argc, char** argv);

std::vector<std::vector<std::string>> parse_query_file(const std::string& filename);

std::unique_ptr<AbstractGraph> create_graph(GraphRepresentation representation);
std::string to_string(OutputMode mode);
std::string to_string(GraphRepresentation representation);

#endif
