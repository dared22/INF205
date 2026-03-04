#include "Node.h"
#include "edge.h"

#include <algorithm>
#include <utility>

Node::Node(std::string label) : label_(std::move(label)) {}

const std::string& Node::label() const {
    return label_;
}

void Node::addIncidentEdge(Edge* edge) {
    if (edge != nullptr) {
        incidentEdges_.push_back(edge);
    }
}

void Node::removeIncidentEdge(const Edge* edge) {
    incidentEdges_.erase(
        std::remove(incidentEdges_.begin(), incidentEdges_.end(), edge),
        incidentEdges_.end()
    );
}

const std::vector<Edge*>& Node::incidentEdges() const {
    return incidentEdges_;
}
