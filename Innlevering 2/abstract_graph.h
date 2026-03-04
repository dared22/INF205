#ifndef ABSTRACTGRAPH_H
#define ABSTRACTGRAPH_H

#include <string>
#include <vector>

class Node;
class Edge;

class AbstractGraph {
public:
    virtual ~AbstractGraph() {}

    virtual void insert_edge(std::string node_a_label,
                            std::string edge_label,
                            std::string node_b_label) = 0;

    virtual void disconnect(std::string node_a_label,
                           std::string node_b_label) = 0;

    virtual void remove_node(std::string node_label) = 0;

    virtual Node* find_node(const std::string& label) const = 0;

    virtual const std::vector<Node*>& nodes() const = 0;
    virtual const std::vector<Edge*>& edges() const = 0;

    virtual void save(const std::string& filename) = 0;
    virtual void load(const std::string& filename) = 0;
};

#endif