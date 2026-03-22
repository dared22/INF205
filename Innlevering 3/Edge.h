#ifndef EDGE_H
#define EDGE_H

#include <string>

class Node;

class Edge {
public:
    Edge(std::string label, Node* source_node, Node* target_node);

    const std::string& label() const;
    Node* source_node() const;
    Node* target_node() const;

private:
    std::string label_;
    Node* source_node_;
    Node* target_node_;
};

#endif
