#include "Graph.h"

#include "Node.h"
#include "edge.h"

#include <utility>

#include <fstream>
#include <iostream>



Node* Graph::findOrCreateNode(const std::string& label) {
    const auto found = nodesByLabel_.find(label);
    if (found != nodesByLabel_.end()) {
        return found->second;
    }

    Node* node = new Node(label);
    nodesByLabel_[label] = node;
    nodes_.push_back(node);
    return node;
}

Graph::~Graph() {
    for (Edge* edge : edges_) {
        delete edge;
    }

    for (Node* node : nodes_) {
        delete node;
    }
}

void Graph::insert_edge(std::string node_a_label, std::string edge_label, std::string node_b_label) {
    Node* sourceNode = findOrCreateNode(node_a_label);
    Node* targetNode = findOrCreateNode(node_b_label);

    Edge* edge = new Edge(std::move(edge_label), sourceNode, targetNode);
    edges_.push_back(edge);

    sourceNode->addIncidentEdge(edge);
    if (targetNode != sourceNode) {
        targetNode->addIncidentEdge(edge);
    }
}

void Graph::save(const std::string& filename) {
    std::ofstream file(filename);

    for (Edge* edge : edges_) {
        file << edge->sourceNode()->label() << " "
             << edge->label() << " "
             << edge->targetNode()->label() << std::endl;
    }

    file.close();
}

void Graph::load(const std::string& filename) {
    std::ifstream file(filename);
    std::string sourceLabel, edgeLabel, targetLabel;

    while (file >> sourceLabel >> edgeLabel >> targetLabel) {
        insert_edge(sourceLabel, edgeLabel, targetLabel);
    }

    file.close();
}

Node* Graph::find_node(const std::string& label) const {
    const auto found = nodesByLabel_.find(label);
    if (found == nodesByLabel_.end()) {
        return nullptr;
    }
    return found->second;
}

const std::vector<Node*>& Graph::nodes() const {
    return nodes_;
}

const std::vector<Edge*>& Graph::edges() const {
    return edges_;


}
