#ifndef MATRIX_GRAPH_H
#define MATRIX_GRAPH_H

#include "AbstractGraph.h"

#include "Node.h"

#include <map>
#include <string>
#include <vector>

class Edge;

class MatrixGraph : public AbstractGraph {
public:
    MatrixGraph() = default;
    ~MatrixGraph() override;
    MatrixGraph(const MatrixGraph& other);
    MatrixGraph& operator=(const MatrixGraph& other);
    MatrixGraph(MatrixGraph&& other) noexcept;
    MatrixGraph& operator=(MatrixGraph&& other) noexcept;

    void insert_edge(
        std::string node_a_label,
        std::string edge_label,
        std::string node_b_label
    ) override;

    void disconnect(std::string node_a_label, std::string node_b_label) override;
    void remove_node(std::string node_label) override;

    Node* find_node(const std::string& label) const override;
    const std::vector<Node*>& nodes() const override;
    const std::vector<Edge*>& edges() const override;

    void save(const std::string& filename) override;
    void load(const std::string& filename) override;

private:
    int findOrCreateNodeIndex(const std::string& label);
    void expandMatrix();
    void removeNodeByIndex(int index);
    void removeIfIsolated(int index);
    bool hasConnections(int index);

    std::vector<Node*> nodes_;
    std::map<std::string, int> labelToIndex_;
    std::vector<std::vector<std::vector<std::string>>> matrix_;
    std::vector<Edge*> edges_;
};

#endif
