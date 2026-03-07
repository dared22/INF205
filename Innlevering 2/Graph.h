#ifndef GRAPH_H
#define GRAPH_H

#include "abstract_graph.h"
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
    void removeEdge(Edge* edge);
    void removeIfIsolated(Node* node);

public:
    Graph() = default;                          // default constructor
    ~Graph() override;                          // destructor
    Graph(const Graph& other);                  // copy constructor
    Graph& operator=(const Graph& other);       // copy assignment
    Graph(Graph&& other) noexcept;              // move constructor
    Graph& operator=(Graph&& other) noexcept;   // move assignment

    void insert_edge(std::string node_a_label,
                    std::string edge_label,
                    std::string node_b_label) override;

    Node* find_node(const std::string& label) const override;
    
    const std::vector<Node*>& nodes() const override;
    const std::vector<Edge*>& edges() const override;

    void save(const std::string& filename) override;
    void load(const std::string& filename) override;
    void disconnect(std::string node_a_label, std::string node_b_label) override;
    void remove_node(std::string node_label) override;
};

#endif
