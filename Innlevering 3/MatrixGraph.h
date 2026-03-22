#ifndef MATRIX_GRAPH_H
#define MATRIX_GRAPH_H

#include "AbstractGraph.h"

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

class Edge;
class Node;

class MatrixGraph : public AbstractGraph {
public:
    ~MatrixGraph() override;

    void insert_edge(
        const std::string& source_label,
        const std::string& edge_label,
        const std::string& target_label
    ) override;

    void load(const std::string& filename) override;

    Node* find_node(const std::string& label) const override;
    const std::vector<Node*>& nodes() const override;
    const std::vector<Edge*>& edges() const override;

private:
    std::size_t find_or_create_node_index(const std::string& label);
    void expand_matrix();

    std::vector<Node*> nodes_;
    std::vector<Edge*> edges_;
    std::unordered_map<std::string, std::size_t> node_index_by_label_;
    std::vector<std::vector<std::vector<Edge*>>> matrix_;
};

#endif
