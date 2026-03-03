#ifndef GRAPH_H
#define GRAPH_H

#include "abstractgraph.h"
#include <string>
#include <vector>
#include <map>

class Node;
class Edge;

class Graph : public AbstractGraph {
private:
    std::vector<Node*> nodes_;
    std::vector<Edge*> edges_;
    std::map<std::string, Node*> nodesByLabel_;

    Node* findOrCreateNode(const std::string& label);

public:
    ~Graph() override;

    void insert_edge(std::string node_a_label,
                    std::string edge_label,
                    std::string node_b_label) override;

    Node* find_node(const std::string& label) const override;

    const std::vector<Node*>& nodes() const override;
    const std::vector<Edge*>& edges() const override;

    void save(const std::string& filename) override;
    void load(const std::string& filename) override;
};

#endif