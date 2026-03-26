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

bool is_flag(const std::string& value) {
    return !value.empty() && value.front() == '-';
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
    if (argc < 2) {
        throw std::runtime_error(
            "Usage: ./graph-benchmark-scc [<number of nodes> <number of edges>] <graph file> "
            "[--silent|--verbose] [--linked|--matrix]"
        );
    }

    CommonCliOptions options;
    std::vector<std::string> positional_arguments;
    std::vector<std::string> flags;

    for (int index = 1; index < argc; ++index) {
        const std::string argument = argv[index];
        if (is_flag(argument)) {
            flags.push_back(argument);
        } else {
            positional_arguments.push_back(argument);
        }
    }

    if (positional_arguments.size() == 3) {
        options.expected_nodes = parse_positive_long(positional_arguments[0], "number of nodes");
        options.expected_edges = parse_positive_long(positional_arguments[1], "number of edges");
        options.graph_file = positional_arguments[2];
        options.counts_provided = true;
    } else if (positional_arguments.size() == 1) {
        options.graph_file = positional_arguments[0];
    } else {
        throw std::runtime_error(
            "Usage: ./graph-benchmark-scc [<number of nodes> <number of edges>] <graph file> "
            "[--silent|--verbose] [--linked|--matrix]"
        );
    }

    for (const std::string& flag : flags) {
        apply_optional_flag(flag, options.output_mode, options.representation);
    }

    return options;
}

DiamondCliOptions parse_diamond_cli(int argc, char** argv) {
    if (argc < 3) {
        throw std::runtime_error(
            "Usage: ./graph-benchmark-diamond [<number of nodes> <number of edges>] "
            "<graph file> <query file> [--silent|--verbose] [--linked|--matrix]"
        );
    }

    DiamondCliOptions options;
    std::vector<std::string> positional_arguments;
    std::vector<std::string> flags;

    for (int index = 1; index < argc; ++index) {
        const std::string argument = argv[index];
        if (is_flag(argument)) {
            flags.push_back(argument);
        } else {
            positional_arguments.push_back(argument);
        }
    }

    if (positional_arguments.size() == 4) {
        options.expected_nodes = parse_positive_long(positional_arguments[0], "number of nodes");
        options.expected_edges = parse_positive_long(positional_arguments[1], "number of edges");
        options.graph_file = positional_arguments[2];
        options.query_file = positional_arguments[3];
        options.counts_provided = true;
    } else if (positional_arguments.size() == 2) {
        options.graph_file = positional_arguments[0];
        options.query_file = positional_arguments[1];
    } else {
        throw std::runtime_error(
            "Usage: ./graph-benchmark-diamond [<number of nodes> <number of edges>] "
            "<graph file> <query file> [--silent|--verbose] [--linked|--matrix]"
        );
    }

    for (const std::string& flag : flags) {
        apply_optional_flag(flag, options.output_mode, options.representation);
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
