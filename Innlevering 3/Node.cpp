#include "Node.h"

#include "Edge.h"

#include <utility>

Node::Node(std::string label) : label_(std::move(label)) {}

const std::string& Node::label() const {
    return label_;
}

void Node::add_incident_edge(Edge* edge) {
    if (edge != nullptr) {
        incident_edges_.push_back(edge);
    }
}

void Node::add_outgoing_edge(Edge* edge) {
    if (edge != nullptr) {
        outgoing_edges_.push_back(edge);
    }
}

const std::vector<Edge*>& Node::incident_edges() const {
    return incident_edges_;
}

const std::vector<Edge*>& Node::outgoing_edges() const {
    return outgoing_edges_;
}
