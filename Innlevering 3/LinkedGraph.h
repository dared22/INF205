#ifndef LINKED_GRAPH_H
#define LINKED_GRAPH_H

#include "AbstractGraph.h"

#include <string>
#include <unordered_map>
#include <vector>

class Edge;
class Node;

class LinkedGraph : public AbstractGraph {
public:
    ~LinkedGraph() override;

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
    Node* find_or_create_node(const std::string& label);

    std::vector<Node*> nodes_;
    std::vector<Edge*> edges_;
    std::unordered_map<std::string, Node*> nodes_by_label_;
};

#endif
