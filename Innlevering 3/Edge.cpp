#include "Edge.h"

#include <utility>

Edge::Edge(std::string label, Node* source_node, Node* target_node)
    : label_(std::move(label)),
      source_node_(source_node),
      target_node_(target_node) {}

const std::string& Edge::label() const {
    return label_;
}

Node* Edge::source_node() const {
    return source_node_;
}

Node* Edge::target_node() const {
    return target_node_;
}
