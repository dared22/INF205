#include "MatrixGraph.h"

#include "Edge.h"
#include "Node.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

namespace {

void parse_and_insert_line(
    const std::string& line,
    AbstractGraph& graph,
    const std::string& filename,
    std::size_t line_number
) {
    if (line.empty()) {
        return;
    }

    std::istringstream iss(line);
    std::string source_label;
    std::string edge_label;
    std::string target_label;
    std::string trailing_token;

    if (!(iss >> source_label >> edge_label >> target_label)) {
        return;
    }

    if (!target_label.empty() && target_label.back() == '.') {
        target_label.pop_back();
    } else if (iss >> trailing_token && trailing_token == ".") {
    } else {
        throw std::runtime_error(
            "Invalid triple notation in " + filename + " at line " + std::to_string(line_number)
        );
    }

    graph.insert_edge(source_label, edge_label, target_label);
}

}  // namespace

MatrixGraph::~MatrixGraph() {
    for (Edge* edge : edges_) {
        delete edge;
    }

    for (Node* node : nodes_) {
        delete node;
    }
}

void MatrixGraph::insert_edge(
    const std::string& source_label,
    const std::string& edge_label,
    const std::string& target_label
) {
    const std::size_t source_index = find_or_create_node_index(source_label);
    const std::size_t target_index = find_or_create_node_index(target_label);

    Edge* edge = new Edge(edge_label, nodes_[source_index], nodes_[target_index]);
    edges_.push_back(edge);
    matrix_[source_index][target_index].push_back(edge);

    nodes_[source_index]->add_incident_edge(edge);
    nodes_[source_index]->add_outgoing_edge(edge);
    if (source_index != target_index) {
        nodes_[target_index]->add_incident_edge(edge);
    }
}

void MatrixGraph::load(const std::string& filename) {
    std::ifstream input(filename);
    if (!input) {
        throw std::runtime_error("Could not open graph file: " + filename);
    }

    std::string line;
    std::size_t line_number = 0;
    while (std::getline(input, line)) {
        ++line_number;
        parse_and_insert_line(line, *this, filename, line_number);
    }
}

Node* MatrixGraph::find_node(const std::string& label) const {
    const auto it = node_index_by_label_.find(label);
    return it == node_index_by_label_.end() ? nullptr : nodes_[it->second];
}

const std::vector<Node*>& MatrixGraph::nodes() const {
    return nodes_;
}

const std::vector<Edge*>& MatrixGraph::edges() const {
    return edges_;
}

std::size_t MatrixGraph::find_or_create_node_index(const std::string& label) {
    const auto it = node_index_by_label_.find(label);
    if (it != node_index_by_label_.end()) {
        return it->second;
    }

    const std::size_t index = nodes_.size();
    nodes_.push_back(new Node(label));
    node_index_by_label_[label] = index;
    expand_matrix();
    return index;
}

void MatrixGraph::expand_matrix() {
    const std::size_t new_size = nodes_.size();

    for (auto& row : matrix_) {
        row.resize(new_size);
    }

    matrix_.resize(new_size, std::vector<std::vector<Edge*>>(new_size));
}
