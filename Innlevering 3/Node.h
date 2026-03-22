#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>

class Edge;

class Node {
public:
    explicit Node(std::string label);

    const std::string& label() const;

    void add_incident_edge(Edge* edge);
    void add_outgoing_edge(Edge* edge);

    const std::vector<Edge*>& incident_edges() const;
    const std::vector<Edge*>& outgoing_edges() const;

private:
    std::string label_;
    std::vector<Edge*> incident_edges_;
    std::vector<Edge*> outgoing_edges_;
};

#endif
