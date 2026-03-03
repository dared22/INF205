#include <iostream>
#include "Graph.h"
#include "Node.h"
#include "edge.h"

int main() {

    Graph g;

    // --- Test inserting edges ---
    std::cout << "=== Inserting Edges ===" << std::endl;
    g.insert_edge("Paris", "road", "London");
    g.insert_edge("London", "train", "Berlin");
    g.insert_edge("Berlin", "flight", "Paris");
    g.insert_edge("Paris", "flight", "Paris");  // self-loop
    g.insert_edge("Paris", "bus", "London");    // duplicate route
    std::cout << "Inserted 5 edges." << std::endl;

    // --- Test node count ---
    std::cout << "\n=== Nodes ===" << std::endl;
    std::cout << "Total nodes: " << g.nodes().size() << std::endl;
    for (Node* node : g.nodes()) {
        std::cout << "  Node: " << node->label()
                  << " (incident edges: " << node->incidentEdges().size() << ")"
                  << std::endl;
    }

    // --- Test edge count ---
    std::cout << "\n=== Edges ===" << std::endl;
    std::cout << "Total edges: " << g.edges().size() << std::endl;
    for (Edge* edge : g.edges()) {
        std::cout << "  " << edge->sourceNode()->label()
                  << " --" << edge->label() << "--> "
                  << edge->targetNode()->label() << std::endl;
    }

    // --- Test find_node ---
    std::cout << "\n=== Finding Nodes ===" << std::endl;
    Node* found = g.find_node("Paris");
    if (found) {
        std::cout << "Found node: " << found->label() << std::endl;
    }

    Node* notFound = g.find_node("Tokyo");
    if (!notFound) {
        std::cout << "Node 'Tokyo' not found (expected)." << std::endl;
    }

    // --- Test incident edges of a node ---
    std::cout << "\n=== Incident Edges of Paris ===" << std::endl;
    Node* paris = g.find_node("Paris");
    for (Edge* edge : paris->incidentEdges()) {
        std::cout << "  " << edge->sourceNode()->label()
                  << " --" << edge->label() << "--> "
                  << edge->targetNode()->label() << std::endl;
    }

    // --- Test incident nodes of an edge ---
    std::cout << "\n=== Incident Nodes of First Edge ===" << std::endl;
    Edge* firstEdge = g.edges()[0];
    for (Node* node : firstEdge->incidentNodes()) {
        std::cout << "  " << node->label() << std::endl;
    }

    // --- Test save to file ---
    std::cout << "\n=== Saving to file ===" << std::endl;
    g.save("graph.txt");
    std::cout << "Saved graph to graph.txt" << std::endl;

    // --- Test load from file ---
    std::cout << "\n=== Loading from file ===" << std::endl;
    Graph g2;
    g2.load("graph.txt");
    std::cout << "Loaded graph with "
              << g2.nodes().size() << " nodes and "
              << g2.edges().size() << " edges." << std::endl;

    for (Edge* edge : g2.edges()) {
        std::cout << "  " << edge->sourceNode()->label()
                  << " --" << edge->label() << "--> "
                  << edge->targetNode()->label() << std::endl;
    }

    return 0;
}
