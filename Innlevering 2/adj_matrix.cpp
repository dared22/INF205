#include "adj_matrix.h"
#include "edge.h"

#include <fstream>
#include <sstream>


int MatrixGraph::findOrCreateNodeIndex(const std::string& label) {
    auto found = labelToIndex_.find(label);
    if (found != labelToIndex_.end()) {
        return found->second;
    }

    int newIndex = nodes_.size();
    Node* node = new Node(label);
    nodes_.push_back(node);
    labelToIndex_[label] = newIndex;

    expandMatrix();

    return newIndex;
}

void MatrixGraph::expandMatrix() {
    int newSize = nodes_.size();

    for (auto& row : matrix_) {
        row.push_back({});
    }

    matrix_.push_back(std::vector<std::vector<std::string>>(newSize));
}


MatrixGraph::~MatrixGraph() {
    for (Edge* edge : edges_) {
        delete edge;
    }
    for (Node* node : nodes_) {
        delete node;
    }
}


void MatrixGraph::insert_edge(std::string node_a_label,
                              std::string edge_label,
                              std::string node_b_label) {
    int sourceIndex = findOrCreateNodeIndex(node_a_label);
    int targetIndex = findOrCreateNodeIndex(node_b_label);

    matrix_[sourceIndex][targetIndex].push_back(edge_label);

    Edge* edge = new Edge(edge_label, nodes_[sourceIndex], nodes_[targetIndex]);
    edges_.push_back(edge);

    nodes_[sourceIndex]->addIncidentEdge(edge);
    if (sourceIndex != targetIndex) {
        nodes_[targetIndex]->addIncidentEdge(edge);
    }
}

Node* MatrixGraph::find_node(const std::string& label) const {
    auto found = labelToIndex_.find(label);
    if (found == labelToIndex_.end()) {
        return nullptr;
    }
    return nodes_[found->second];
}

const std::vector<Node*>& MatrixGraph::nodes() const {
    return nodes_;
}

const std::vector<Edge*>& MatrixGraph::edges() const {
    return edges_;
}


void MatrixGraph::save(const std::string& filename) {
    std::ofstream file(filename);

    for (int i = 0; i < (int)nodes_.size(); i++) {
        for (int j = 0; j < (int)nodes_.size(); j++) {
            for (const std::string& edgeLabel : matrix_[i][j]) {
                file << nodes_[i]->label() << " "
                     << edgeLabel << " "
                     << nodes_[j]->label() << "." << std::endl;
            }
        }
    }

    file.close();
}

void MatrixGraph::load(const std::string& filename) {
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

bool MatrixGraph::hasConnections(int index) {
    int size = nodes_.size();

    // Check if this node has any outgoing edges
    for (int j = 0; j < size; j++) {
        if (!matrix_[index][j].empty()) {
            return true;
        }
    }

    // Check if this node has any incoming edges
    for (int i = 0; i < size; i++) {
        if (!matrix_[i][index].empty()) {
            return true;
        }
    }

    return false;
}

void MatrixGraph::removeNodeByIndex(int index) {
    // Get the node
    Node* node = nodes_[index];

    // Remove from label map
    labelToIndex_.erase(node->label());

    // Remove row and column from matrix
    matrix_.erase(matrix_.begin() + index);
    for (auto& row : matrix_) {
        row.erase(row.begin() + index);
    }

    // Remove from nodes list
    nodes_.erase(nodes_.begin() + index);

    // Update label-to-index map for shifted nodes
    for (int i = index; i < (int)nodes_.size(); i++) {
        labelToIndex_[nodes_[i]->label()] = i;
    }

    // Remove all Edge objects involving this node
    std::vector<Edge*> remainingEdges;
    for (Edge* edge : edges_) {
        if (edge->sourceNode() == node || edge->targetNode() == node) {
            delete edge;
        } else {
            remainingEdges.push_back(edge);
        }
    }
    edges_ = remainingEdges;

    // Free the node
    delete node;
}

void MatrixGraph::removeIfIsolated(int index) {
    if (index < 0 || index >= (int)nodes_.size()) {
        return;
    }

    if (!hasConnections(index)) {
        removeNodeByIndex(index);
    }
}

void MatrixGraph::disconnect(std::string node_a_label, std::string node_b_label) {
    // Find both nodes
    auto foundA = labelToIndex_.find(node_a_label);
    auto foundB = labelToIndex_.find(node_b_label);

    if (foundA == labelToIndex_.end() || foundB == labelToIndex_.end()) {
        return;
    }

    int indexA = foundA->second;
    int indexB = foundB->second;
    Node* nodeA = nodes_[indexA];
    Node* nodeB = nodes_[indexB];

    // Clear the edge labels in the matrix
    matrix_[indexA][indexB].clear();

    // Remove the corresponding Edge objects
    std::vector<Edge*> remainingEdges;
    for (Edge* edge : edges_) {
        if (edge->sourceNode() == nodeA && edge->targetNode() == nodeB) {
            // Remove from node incidence lists
            nodeA->removeIncidentEdge(edge);
            nodeB->removeIncidentEdge(edge);
            delete edge;
        } else {
            remainingEdges.push_back(edge);
        }
    }
    edges_ = remainingEdges;

    // Clean up isolated nodes
    // Check B first since removing A could shift B's index
    if (indexA != indexB) {
        if (!hasConnections(indexB)) {
            removeNodeByIndex(indexB);
            // Refresh indexA since it may have shifted
            auto refreshA = labelToIndex_.find(node_a_label);
            if (refreshA != labelToIndex_.end()) {
                indexA = refreshA->second;
                if (!hasConnections(indexA)) {
                    removeNodeByIndex(indexA);
                }
            }
        } else if (!hasConnections(indexA)) {
            removeNodeByIndex(indexA);
        }
    } else {
        if (!hasConnections(indexA)) {
            removeNodeByIndex(indexA);
        }
    }
}

void MatrixGraph::remove_node(std::string node_label) {
    auto found = labelToIndex_.find(node_label);
    if (found == labelToIndex_.end()) {
        return;
    }

    int index = found->second;

    std::vector<std::string> neighborLabels;
    int size = nodes_.size();
    for (int j = 0; j < size; j++) {
        if (j != index && !matrix_[index][j].empty()) {
            neighborLabels.push_back(nodes_[j]->label());
        }
    }
    for (int i = 0; i < size; i++) {
        if (i != index && !matrix_[i][index].empty()) {
            neighborLabels.push_back(nodes_[i]->label());
        }
    }

    removeNodeByIndex(index);

    for (const std::string& label : neighborLabels) {
        auto neighborFound = labelToIndex_.find(label);
        if (neighborFound != labelToIndex_.end()) {
            removeIfIsolated(neighborFound->second);
        }
    }
}
