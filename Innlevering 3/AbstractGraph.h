#ifndef ABSTRACT_GRAPH_H
#define ABSTRACT_GRAPH_H

#include <string>
#include <vector>

class Edge;
class Node;

class AbstractGraph {
public:
    virtual ~AbstractGraph() = default;

    virtual void insert_edge(
        const std::string& source_label,
        const std::string& edge_label,
        const std::string& target_label
    ) = 0;

    virtual void load(const std::string& filename) = 0;

    virtual Node* find_node(const std::string& label) const = 0;
    virtual const std::vector<Node*>& nodes() const = 0;
    virtual const std::vector<Edge*>& edges() const = 0;
};

#endif
