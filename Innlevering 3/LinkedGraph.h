#ifndef LINKED_GRAPH_H
#define LINKED_GRAPH_H

#include "AbstractGraph.h"

#include <map>
#include <string>
#include <vector>

class Edge;
class Node;

class LinkedGraph : public AbstractGraph {
public:
    LinkedGraph() = default;
    ~LinkedGraph() override;
    LinkedGraph(const LinkedGraph& other);
    LinkedGraph& operator=(const LinkedGraph& other);
    LinkedGraph(LinkedGraph&& other) noexcept;
    LinkedGraph& operator=(LinkedGraph&& other) noexcept;

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
    Node* findOrCreateNode(const std::string& label);
    void removeEdge(Edge* edge);
    void removeIfIsolated(Node* node);

    std::vector<Node*> nodes_;
    std::vector<Edge*> edges_;
    std::map<std::string, Node*> nodesByLabel_;
};

#endif
