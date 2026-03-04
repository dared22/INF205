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
             << edge->targetNode()->label() << "." << std::endl;
    }

    file.close();
}

void Graph::load(const std::string& filename) {
    std::ifstream file(filename);
    std::string sourceLabel, edgeLabel, targetLabel;

    while (file >> sourceLabel >> edgeLabel >> targetLabel) {
        if (!targetLabel.empty() && targetLabel.back() == '.') {
            targetLabel.pop_back();
        }
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

void Graph::removeEdge(Edge* edge) {
    // Remove the edge from both nodes' incidence lists
    edge->sourceNode()->removeIncidentEdge(edge);
    edge->targetNode()->removeIncidentEdge(edge);

    // Remove from the graph's edge list
    edges_.erase(
        std::remove(edges_.begin(), edges_.end(), edge),
        edges_.end()
    );

    // Free the memory
    delete edge;
}

void Graph::removeIfIsolated(Node* node) {
    // If the node still has edges, keep it
    if (!node->incidentEdges().empty()) {
        return;
    }

    // Remove from the label map
    nodesByLabel_.erase(node->label());

    // Remove from the nodes list
    nodes_.erase(
        std::remove(nodes_.begin(), nodes_.end(), node),
        nodes_.end()
    );

    // Free the memory
    delete node;
}

void Graph::disconnect(std::string node_a_label, std::string node_b_label) {
    Node* source = find_node(node_a_label);
    Node* target = find_node(node_b_label);

    // If either node doesn't exist, nothing to do
    if (!source || !target) {
        return;
    }

    // Collect all edges from source to target
    std::vector<Edge*> toRemove;
    for (Edge* edge : edges_) {
        if (edge->sourceNode() == source && edge->targetNode() == target) {
            toRemove.push_back(edge);
        }
    }

    // Remove them
    for (Edge* edge : toRemove) {
        removeEdge(edge);
    }

    // Clean up isolated nodes
    removeIfIsolated(source);
    if (target != source) {
        removeIfIsolated(target);
    }
}

void Graph::remove_node(std::string node_label) {
    Node* node = find_node(node_label);

    // If the node doesn't exist, nothing to do
    if (!node) {
        return;
    }

    // Collect all incident edges
    std::vector<Edge*> toRemove(node->incidentEdges().begin(),
                                node->incidentEdges().end());

    // Collect neighbors before removing edges
    std::vector<Node*> neighbors;
    for (Edge* edge : toRemove) {
        if (edge->sourceNode() != node) {
            neighbors.push_back(edge->sourceNode());
        }
        if (edge->targetNode() != node) {
            neighbors.push_back(edge->targetNode());
        }
    }

    // Remove all incident edges
    for (Edge* edge : toRemove) {
        removeEdge(edge);
    }

    // Remove the node itself
    nodesByLabel_.erase(node->label());
    nodes_.erase(
        std::remove(nodes_.begin(), nodes_.end(), node),
        nodes_.end()
    );
    delete node;

    // Clean up any neighbors that became isolated
    for (Node* neighbor : neighbors) {
        removeIfIsolated(neighbor);
    }
}

const std::vector<Node*>& Graph::nodes() const {
    return nodes_;
}

const std::vector<Edge*>& Graph::edges() const {
    return edges_;


}
