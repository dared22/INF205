#include "Cli.h"

#include "AbstractGraph.h"
#include "LinkedGraph.h"
#include "MatrixGraph.h"

#include <cstdlib>
#include <fstream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

long parse_positive_long(const std::string& value, const std::string& name) {
    char* end = nullptr;
    const long parsed = std::strtol(value.c_str(), &end, 10);
    if (end == value.c_str() || *end != '\0' || parsed <= 0) {
        throw std::runtime_error(name + " must be a positive integer, got: " + value);
    }
    return parsed;
}

void apply_optional_flag(
    const std::string& flag,
    OutputMode& output_mode,
    GraphRepresentation& representation
) {
    if (flag == "--silent" || flag == "-s") {
        output_mode = OutputMode::Silent;
        return;
    }
    if (flag == "--verbose" || flag == "-v") {
        output_mode = OutputMode::Verbose;
        return;
    }
    if (flag == "--linked") {
        representation = GraphRepresentation::Linked;
        return;
    }
    if (flag == "--matrix") {
        representation = GraphRepresentation::Matrix;
        return;
    }

    throw std::runtime_error("Unknown flag: " + flag);
}

std::vector<std::string> parse_query_line(const std::string& line, const std::string& filename, std::size_t line_number) {
    std::istringstream iss(line);
    std::vector<std::string> labels;
    std::string token;
    bool terminated = false;

    while (iss >> token) {
        if (!token.empty() && token.back() == '.') {
            token.pop_back();
            if (!token.empty()) {
                labels.push_back(token);
            }
            terminated = true;
            break;
        }
        labels.push_back(token);
    }

    if (!terminated) {
        throw std::runtime_error(
            "Invalid query file format in " + filename + " at line " + std::to_string(line_number)
        );
    }

    return labels;
}

}  // namespace

CommonCliOptions parse_scc_cli(int argc, char** argv) {
    if (argc < 4) {
        throw std::runtime_error(
            "Usage: ./graph-benchmark-scc <number of nodes> <number of edges> <graph file> "
            "[--silent|--verbose] [--linked|--matrix]"
        );
    }

    CommonCliOptions options;
    options.expected_nodes = parse_positive_long(argv[1], "number of nodes");
    options.expected_edges = parse_positive_long(argv[2], "number of edges");
    options.graph_file = argv[3];

    for (int index = 4; index < argc; ++index) {
        apply_optional_flag(argv[index], options.output_mode, options.representation);
    }

    return options;
}

DiamondCliOptions parse_diamond_cli(int argc, char** argv) {
    if (argc < 5) {
        throw std::runtime_error(
            "Usage: ./graph-benchmark-diamond <number of nodes> <number of edges> "
            "<graph file> <query file> [--silent|--verbose] [--linked|--matrix]"
        );
    }

    DiamondCliOptions options;
    options.expected_nodes = parse_positive_long(argv[1], "number of nodes");
    options.expected_edges = parse_positive_long(argv[2], "number of edges");
    options.graph_file = argv[3];
    options.query_file = argv[4];

    for (int index = 5; index < argc; ++index) {
        apply_optional_flag(argv[index], options.output_mode, options.representation);
    }

    return options;
}

std::vector<std::vector<std::string>> parse_query_file(const std::string& filename) {
    std::ifstream input(filename);
    if (!input) {
        throw std::runtime_error("Could not open query file: " + filename);
    }

    std::vector<std::vector<std::string>> sequences;
    std::string line;
    std::size_t line_number = 0;
    while (std::getline(input, line)) {
        ++line_number;
        if (line.empty()) {
            continue;
        }

        std::vector<std::string> parsed = parse_query_line(line, filename, line_number);
        if (!parsed.empty()) {
            sequences.push_back(std::move(parsed));
        }
    }

    if (sequences.size() != 2) {
        throw std::runtime_error(
            "Query file must contain exactly two non-empty label sequences ending with '.'"
        );
    }

    return sequences;
}

std::unique_ptr<AbstractGraph> create_graph(GraphRepresentation representation) {
    if (representation == GraphRepresentation::Matrix) {
        return std::make_unique<MatrixGraph>();
    }
    return std::make_unique<LinkedGraph>();
}

std::string to_string(OutputMode mode) {
    return mode == OutputMode::Silent ? "silent" : "verbose";
}

std::string to_string(GraphRepresentation representation) {
    return representation == GraphRepresentation::Matrix ? "matrix" : "linked";
}
