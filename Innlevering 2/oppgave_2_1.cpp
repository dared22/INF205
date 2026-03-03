#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

class Edge; // Forward decleration

class Node {
    public:
        std::string label;
        std::vector<Edge*> connecting_edges; // list of Edges connected to this node

        Node(const std::string& lbl) : label(lbl) {}
};

class Edge {
    public: 
        std::string label;
        Node* source;
        Node* target;

        Edge(const std::string& lbl, Node* src, Node* tgt) : label(lbl), source(src), target(tgt) {}
};

class Graph {
    private: 
        std::unordered_map<std::string, std::unique_ptr<Node>> nodes; // Unique nodes with labels as identifiers
        std::vector<std::unique_ptr<Edge>> edges;

    public:
        void insert_edge(std::string node_a_label, std::string edge_label, std::string node_b_label)
        {
            // If no source node exists
            if (nodes.find(node_a_label) == nodes.end()){
                nodes[node_a_label] = std::make_unique<Node>(node_a_label);
            }

            // If no target node exists
            if (nodes.find(node_b_label) == nodes.end()){
                nodes[node_b_label] = std::make_unique<Node>(node_b_label);
            }

            Node* source = nodes[node_a_label].get();
            Node* target = nodes[node_b_label].get();

            // Create new edge
            edges.push_back(std::make_unique<Edge>(edge_label, source, target));
            Edge* new_edge = edges.back().get();

            // Adding new_edge to the connecting_edges list of both nodes
            source->connecting_edges.push_back(new_edge);
            target->connecting_edges.push_back(new_edge);
        }
};