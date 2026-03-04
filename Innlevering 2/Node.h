#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>

class Edge;

class Node {
private:
    std::string label_;
    std::vector<Edge*> incidentEdges_;

public:
    explicit Node(std::string label);
    const std::string& label() const;
    void addIncidentEdge(Edge* edge);
    void removeIncidentEdge(const Edge* edge);
    const std::vector<Edge*>& incidentEdges() const;
};

#endif
