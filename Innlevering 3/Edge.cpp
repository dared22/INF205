#include "Edge.h"

#include "Node.h"

#include <utility>

void Edge::syncIncidentNodes() {
    incidentNodes_.clear();
    incidentNodes_.push_back(sourceNode_);
    incidentNodes_.push_back(targetNode_);
}

Edge::Edge(std::string label, Node* sourceNode, Node* targetNode)
    : label_(std::move(label)),
      sourceNode_(sourceNode),
      targetNode_(targetNode) {
    syncIncidentNodes();
}

const std::string& Edge::label() const {
    return label_;
}

void Edge::setLabel(std::string label) {
    label_ = std::move(label);
}

Node* Edge::sourceNode() const {
    return sourceNode_;
}

Node* Edge::targetNode() const {
    return targetNode_;
}

void Edge::setSourceNode(Node* sourceNode) {
    sourceNode_ = sourceNode;
    syncIncidentNodes();
}

void Edge::setTargetNode(Node* targetNode) {
    targetNode_ = targetNode;
    syncIncidentNodes();
}

const std::vector<Node*>& Edge::incidentNodes() const {
    return incidentNodes_;
}
