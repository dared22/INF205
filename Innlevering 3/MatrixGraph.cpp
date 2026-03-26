#include "MatrixGraph.h"

#include "Edge.h"

#include <fstream>
#include <sstream>

int MatrixGraph::findOrCreateNodeIndex(const std::string& label) {
    const auto found = labelToIndex_.find(label);
    if (found != labelToIndex_.end()) {
        return found->second;
    }

    const int newIndex = static_cast<int>(nodes_.size());
    Node* node = new Node(label);
    nodes_.push_back(node);
    labelToIndex_[label] = newIndex;

    expandMatrix();

    return newIndex;
}

void MatrixGraph::expandMatrix() {
    const int newSize = static_cast<int>(nodes_.size());

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

void MatrixGraph::insert_edge(
    std::string node_a_label,
    std::string edge_label,
    std::string node_b_label
) {
    const int sourceIndex = findOrCreateNodeIndex(node_a_label);
    const int targetIndex = findOrCreateNodeIndex(node_b_label);

    matrix_[sourceIndex][targetIndex].push_back(edge_label);

    Edge* edge = new Edge(edge_label, nodes_[sourceIndex], nodes_[targetIndex]);
    edges_.push_back(edge);

    nodes_[sourceIndex]->addIncidentEdge(edge);
    if (sourceIndex != targetIndex) {
        nodes_[targetIndex]->addIncidentEdge(edge);
    }
}

Node* MatrixGraph::find_node(const std::string& label) const {
    const auto found = labelToIndex_.find(label);
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

    for (int i = 0; i < static_cast<int>(nodes_.size()); ++i) {
        for (int j = 0; j < static_cast<int>(nodes_.size()); ++j) {
            for (const std::string& edgeLabel : matrix_[i][j]) {
                file << nodes_[i]->label() << " "
                     << edgeLabel << " "
                     << nodes_[j]->label() << "." << std::endl;
            }
        }
    }
}

void MatrixGraph::load(const std::string& filename) {
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

bool MatrixGraph::hasConnections(int index) {
    const int size = static_cast<int>(nodes_.size());

    for (int j = 0; j < size; ++j) {
        if (!matrix_[index][j].empty()) {
            return true;
        }
    }

    for (int i = 0; i < size; ++i) {
        if (!matrix_[i][index].empty()) {
            return true;
        }
    }

    return false;
}

void MatrixGraph::removeNodeByIndex(int index) {
    Node* node = nodes_[index];

    labelToIndex_.erase(node->label());

    matrix_.erase(matrix_.begin() + index);
    for (auto& row : matrix_) {
        row.erase(row.begin() + index);
    }

    nodes_.erase(nodes_.begin() + index);

    for (int i = index; i < static_cast<int>(nodes_.size()); ++i) {
        labelToIndex_[nodes_[i]->label()] = i;
    }

    std::vector<Edge*> remainingEdges;
    for (Edge* edge : edges_) {
        if (edge->sourceNode() == node || edge->targetNode() == node) {
            delete edge;
        } else {
            remainingEdges.push_back(edge);
        }
    }
    edges_ = remainingEdges;

    delete node;
}

void MatrixGraph::removeIfIsolated(int index) {
    if (index < 0 || index >= static_cast<int>(nodes_.size())) {
        return;
    }

    if (!hasConnections(index)) {
        removeNodeByIndex(index);
    }
}

void MatrixGraph::disconnect(std::string node_a_label, std::string node_b_label) {
    const auto foundA = labelToIndex_.find(node_a_label);
    const auto foundB = labelToIndex_.find(node_b_label);

    if (foundA == labelToIndex_.end() || foundB == labelToIndex_.end()) {
        return;
    }

    int indexA = foundA->second;
    int indexB = foundB->second;
    Node* nodeA = nodes_[indexA];
    Node* nodeB = nodes_[indexB];

    matrix_[indexA][indexB].clear();

    std::vector<Edge*> remainingEdges;
    for (Edge* edge : edges_) {
        if (edge->sourceNode() == nodeA && edge->targetNode() == nodeB) {
            nodeA->removeIncidentEdge(edge);
            nodeB->removeIncidentEdge(edge);
            delete edge;
        } else {
            remainingEdges.push_back(edge);
        }
    }
    edges_ = remainingEdges;

    if (indexA != indexB) {
        if (!hasConnections(indexB)) {
            removeNodeByIndex(indexB);
            const auto refreshA = labelToIndex_.find(node_a_label);
            if (refreshA != labelToIndex_.end()) {
                indexA = refreshA->second;
                if (!hasConnections(indexA)) {
                    removeNodeByIndex(indexA);
                }
            }
        } else if (!hasConnections(indexA)) {
            removeNodeByIndex(indexA);
        }
    } else if (!hasConnections(indexA)) {
        removeNodeByIndex(indexA);
    }
}

void MatrixGraph::remove_node(std::string node_label) {
    const auto found = labelToIndex_.find(node_label);
    if (found == labelToIndex_.end()) {
        return;
    }

    const int index = found->second;

    std::vector<std::string> neighborLabels;
    const int size = static_cast<int>(nodes_.size());
    for (int j = 0; j < size; ++j) {
        if (j != index && !matrix_[index][j].empty()) {
            neighborLabels.push_back(nodes_[j]->label());
        }
    }
    for (int i = 0; i < size; ++i) {
        if (i != index && !matrix_[i][index].empty()) {
            neighborLabels.push_back(nodes_[i]->label());
        }
    }

    removeNodeByIndex(index);

    for (const std::string& label : neighborLabels) {
        const auto neighborFound = labelToIndex_.find(label);
        if (neighborFound != labelToIndex_.end()) {
            removeIfIsolated(neighborFound->second);
        }
    }
}

MatrixGraph::MatrixGraph(const MatrixGraph& other) {
    for (Node* node : other.nodes_) {
        Node* newNode = new Node(node->label());
        nodes_.push_back(newNode);
        labelToIndex_[node->label()] = static_cast<int>(nodes_.size()) - 1;
    }

    matrix_ = other.matrix_;

    for (Edge* edge : other.edges_) {
        Node* sourceNode = nodes_[labelToIndex_[edge->sourceNode()->label()]];
        Node* targetNode = nodes_[labelToIndex_[edge->targetNode()->label()]];
        Edge* newEdge = new Edge(edge->label(), sourceNode, targetNode);
        edges_.push_back(newEdge);
        sourceNode->addIncidentEdge(newEdge);
        if (sourceNode != targetNode) {
            targetNode->addIncidentEdge(newEdge);
        }
    }
}

MatrixGraph& MatrixGraph::operator=(const MatrixGraph& other) {
    if (this != &other) {
        for (Edge* edge : edges_) {
            delete edge;
        }
        for (Node* node : nodes_) {
            delete node;
        }
        nodes_.clear();
        edges_.clear();
        labelToIndex_.clear();
        matrix_.clear();

        for (Node* node : other.nodes_) {
            Node* newNode = new Node(node->label());
            nodes_.push_back(newNode);
            labelToIndex_[node->label()] = static_cast<int>(nodes_.size()) - 1;
        }

        matrix_ = other.matrix_;

        for (Edge* edge : other.edges_) {
            Node* sourceNode = nodes_[labelToIndex_[edge->sourceNode()->label()]];
            Node* targetNode = nodes_[labelToIndex_[edge->targetNode()->label()]];
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

MatrixGraph::MatrixGraph(MatrixGraph&& other) noexcept
    : nodes_(std::move(other.nodes_)),
      labelToIndex_(std::move(other.labelToIndex_)),
      matrix_(std::move(other.matrix_)),
      edges_(std::move(other.edges_)) {}

MatrixGraph& MatrixGraph::operator=(MatrixGraph&& other) noexcept {
    if (this != &other) {
        for (Edge* edge : edges_) {
            delete edge;
        }
        for (Node* node : nodes_) {
            delete node;
        }

        nodes_ = std::move(other.nodes_);
        labelToIndex_ = std::move(other.labelToIndex_);
        matrix_ = std::move(other.matrix_);
        edges_ = std::move(other.edges_);
    }
    return *this;
}
