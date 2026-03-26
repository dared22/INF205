#include "LinkedGraph.h"

#include "Edge.h"
#include "Node.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <utility>

Node* LinkedGraph::findOrCreateNode(const std::string& label) {
    const auto found = nodesByLabel_.find(label);
    if (found != nodesByLabel_.end()) {
        return found->second;
    }

    Node* node = new Node(label);
    nodesByLabel_[label] = node;
    nodes_.push_back(node);
    return node;
}

LinkedGraph::~LinkedGraph() {
    for (Edge* edge : edges_) {
        delete edge;
    }

    for (Node* node : nodes_) {
        delete node;
    }
}

void LinkedGraph::insert_edge(std::string node_a_label, std::string edge_label, std::string node_b_label) {
    Node* sourceNode = findOrCreateNode(node_a_label);
    Node* targetNode = findOrCreateNode(node_b_label);

    Edge* edge = new Edge(std::move(edge_label), sourceNode, targetNode);
    edges_.push_back(edge);

    sourceNode->addIncidentEdge(edge);
    if (targetNode != sourceNode) {
        targetNode->addIncidentEdge(edge);
    }
}

void LinkedGraph::save(const std::string& filename) {
    std::ofstream file(filename);

    for (Edge* edge : edges_) {
        file << edge->sourceNode()->label() << " "
             << edge->label() << " "
             << edge->targetNode()->label() << "." << std::endl;
    }
}

void LinkedGraph::load(const std::string& filename) {
    std::ifstream file(filename);
    std::string sourceLabel;
    std::string edgeLabel;
    std::string targetLabel;

    while (file >> sourceLabel >> edgeLabel >> targetLabel) {
        if (!targetLabel.empty() && targetLabel.back() == '.') {
            targetLabel.pop_back();
        }
        insert_edge(sourceLabel, edgeLabel, targetLabel);
    }
}

Node* LinkedGraph::find_node(const std::string& label) const {
    const auto found = nodesByLabel_.find(label);
    if (found == nodesByLabel_.end()) {
        return nullptr;
    }
    return found->second;
}

void LinkedGraph::removeEdge(Edge* edge) {
    edge->sourceNode()->removeIncidentEdge(edge);
    edge->targetNode()->removeIncidentEdge(edge);

    edges_.erase(
        std::remove(edges_.begin(), edges_.end(), edge),
        edges_.end()
    );

    delete edge;
}

void LinkedGraph::removeIfIsolated(Node* node) {
    if (!node->incidentEdges().empty()) {
        return;
    }

    nodesByLabel_.erase(node->label());
    nodes_.erase(
        std::remove(nodes_.begin(), nodes_.end(), node),
        nodes_.end()
    );

    delete node;
}

void LinkedGraph::disconnect(std::string node_a_label, std::string node_b_label) {
    Node* source = find_node(node_a_label);
    Node* target = find_node(node_b_label);

    if (!source || !target) {
        return;
    }

    std::vector<Edge*> toRemove;
    for (Edge* edge : edges_) {
        if (edge->sourceNode() == source && edge->targetNode() == target) {
            toRemove.push_back(edge);
        }
    }

    for (Edge* edge : toRemove) {
        removeEdge(edge);
    }

    removeIfIsolated(source);
    if (target != source) {
        removeIfIsolated(target);
    }
}

void LinkedGraph::remove_node(std::string node_label) {
    Node* node = find_node(node_label);
    if (!node) {
        return;
    }

    std::vector<Edge*> toRemove(node->incidentEdges().begin(), node->incidentEdges().end());
    std::vector<Node*> neighbors;
    for (Edge* edge : toRemove) {
        if (edge->sourceNode() != node) {
            neighbors.push_back(edge->sourceNode());
        }
        if (edge->targetNode() != node) {
            neighbors.push_back(edge->targetNode());
        }
    }

    for (Edge* edge : toRemove) {
        removeEdge(edge);
    }

    nodesByLabel_.erase(node->label());
    nodes_.erase(
        std::remove(nodes_.begin(), nodes_.end(), node),
        nodes_.end()
    );
    delete node;

    for (Node* neighbor : neighbors) {
        removeIfIsolated(neighbor);
    }
}

const std::vector<Node*>& LinkedGraph::nodes() const {
    return nodes_;
}

const std::vector<Edge*>& LinkedGraph::edges() const {
    return edges_;
}

LinkedGraph::LinkedGraph(const LinkedGraph& other) {
    for (Node* node : other.nodes_) {
        Node* newNode = new Node(node->label());
        nodes_.push_back(newNode);
        nodesByLabel_[node->label()] = newNode;
    }

    for (Edge* edge : other.edges_) {
        Node* sourceNode = nodesByLabel_[edge->sourceNode()->label()];
        Node* targetNode = nodesByLabel_[edge->targetNode()->label()];
        Edge* newEdge = new Edge(edge->label(), sourceNode, targetNode);
        edges_.push_back(newEdge);

        sourceNode->addIncidentEdge(newEdge);
        if (sourceNode != targetNode) {
            targetNode->addIncidentEdge(newEdge);
        }
    }
}

LinkedGraph& LinkedGraph::operator=(const LinkedGraph& other) {
    if (this != &other) {
        for (Edge* edge : edges_) {
            delete edge;
        }
        for (Node* node : nodes_) {
            delete node;
        }
        nodes_.clear();
        edges_.clear();
        nodesByLabel_.clear();

        for (Node* node : other.nodes_) {
            Node* newNode = new Node(node->label());
            nodes_.push_back(newNode);
            nodesByLabel_[node->label()] = newNode;
        }

        for (Edge* edge : other.edges_) {
            Node* sourceNode = nodesByLabel_[edge->sourceNode()->label()];
            Node* targetNode = nodesByLabel_[edge->targetNode()->label()];
            Edge* newEdge = new Edge(edge->label(), sourceNode, targetNode);
            edges_.push_back(newEdge);

            sourceNode->addIncidentEdge(newEdge);
            if (sourceNode != targetNode) {
                targetNode->addIncidentEdge(newEdge);
            }
        }
    }
    return *this;
}

LinkedGraph::LinkedGraph(LinkedGraph&& other) noexcept
    : nodes_(std::move(other.nodes_)),
      edges_(std::move(other.edges_)),
      nodesByLabel_(std::move(other.nodesByLabel_)) {}

LinkedGraph& LinkedGraph::operator=(LinkedGraph&& other) noexcept {
    if (this != &other) {
        for (Edge* edge : edges_) {
            delete edge;
        }
        for (Node* node : nodes_) {
            delete node;
        }

        nodes_ = std::move(other.nodes_);
        edges_ = std::move(other.edges_);
        nodesByLabel_ = std::move(other.nodesByLabel_);
    }
    return *this;
}
