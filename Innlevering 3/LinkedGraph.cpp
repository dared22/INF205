#include "LinkedGraph.h"

#include "Edge.h"
#include "Node.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <utility>

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

LinkedGraph::~LinkedGraph() {
    for (Edge* edge : edges_) {
        delete edge;
    }

    for (Node* node : nodes_) {
        delete node;
    }
}

void LinkedGraph::insert_edge(
    const std::string& source_label,
    const std::string& edge_label,
    const std::string& target_label
) {
    Node* source_node = find_or_create_node(source_label);
    Node* target_node = find_or_create_node(target_label);

    Edge* edge = new Edge(edge_label, source_node, target_node);
    edges_.push_back(edge);

    source_node->add_incident_edge(edge);
    source_node->add_outgoing_edge(edge);
    if (target_node != source_node) {
        target_node->add_incident_edge(edge);
    }
}

void LinkedGraph::load(const std::string& filename) {
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

Node* LinkedGraph::find_node(const std::string& label) const {
    const auto it = nodes_by_label_.find(label);
    return it == nodes_by_label_.end() ? nullptr : it->second;
}

const std::vector<Node*>& LinkedGraph::nodes() const {
    return nodes_;
}

const std::vector<Edge*>& LinkedGraph::edges() const {
    return edges_;
}

Node* LinkedGraph::find_or_create_node(const std::string& label) {
    const auto it = nodes_by_label_.find(label);
    if (it != nodes_by_label_.end()) {
        return it->second;
    }

    Node* node = new Node(label);
    nodes_.push_back(node);
    nodes_by_label_[label] = node;
    return node;
}
