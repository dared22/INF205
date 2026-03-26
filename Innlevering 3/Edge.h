#ifndef EDGE_H
#define EDGE_H

#include <string>
#include <vector>

class Node;

class Edge {
public:
    Edge(std::string label, Node* sourceNode, Node* targetNode);

    const std::string& label() const;
    void setLabel(std::string label);
    Node* sourceNode() const;
    Node* targetNode() const;
    void setSourceNode(Node* sourceNode);
    void setTargetNode(Node* targetNode);
    const std::vector<Node*>& incidentNodes() const;

private:
    void syncIncidentNodes();

    std::string label_;
    Node* sourceNode_;
    Node* targetNode_;
    std::vector<Node*> incidentNodes_;
};

#endif
