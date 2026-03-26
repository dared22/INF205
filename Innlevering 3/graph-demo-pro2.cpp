#include "AbstractGraph.h"
#include "Edge.h"
#include "LinkedGraph.h"
#include "MatrixGraph.h"
#include "Node.h"

#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <utility>

namespace {

void print_graph_summary(const AbstractGraph& graph) {
    std::cout << "Nodes (" << graph.nodes().size() << "):";
    for (const Node* node : graph.nodes()) {
        std::cout << ' ' << node->label() << "[deg=" << node->incidentEdges().size() << "]";
    }
    std::cout << "\nEdges (" << graph.edges().size() << "):\n";
    for (const Edge* edge : graph.edges()) {
        std::cout << "  " << edge->sourceNode()->label()
                  << " -[" << edge->label() << "]-> "
                  << edge->targetNode()->label() << '\n';
    }
}

void demo_pro21() {
    LinkedGraph graph;
    graph.insert_edge("A", "ab", "B");
    graph.insert_edge("A", "ab2", "B");
    graph.insert_edge("B", "bc", "C");
    graph.insert_edge("C", "self", "C");

    std::cout << "Problem 2.1 demo: linked incidence-list graph\n";
    std::cout << "This demo builds a directed graph with duplicate edges and a self-loop.\n";
    print_graph_summary(graph);
}

void demo_pro22() {
    const std::string input_file = ".demo-pro22-input.txt";
    const std::string output_file = ".demo-pro22-output.txt";

    {
        std::ofstream out(input_file);
        out << "A r0 B.\n";
        out << "A r1 B.\n";
        out << "B r2 C.\n";
        out << "C self C.\n";
    }

    LinkedGraph graph;
    graph.load(input_file);
    graph.save(output_file);

    std::ifstream saved(output_file);
    std::ostringstream buffer;
    buffer << saved.rdbuf();

    std::cout << "Problem 2.2 demo: file input and output in triple notation\n";
    std::cout << "Loaded graph from a generated triple file and saved it again.\n";
    print_graph_summary(graph);
    std::cout << "\nSaved output:\n" << buffer.str();

    std::remove(input_file.c_str());
    std::remove(output_file.c_str());
}

void populate_via_interface(AbstractGraph& graph) {
    graph.insert_edge("n0", "r0", "n1");
    graph.insert_edge("n1", "r1", "n2");
    graph.insert_edge("n2", "r2", "n0");
}

void run_interface_demo(std::unique_ptr<AbstractGraph> graph, const std::string& name) {
    populate_via_interface(*graph);
    std::cout << name << " through AbstractGraph:\n";
    print_graph_summary(*graph);
    std::cout << '\n';
}

void demo_pro23() {
    std::cout << "Problem 2.3 demo: abstract interface used with two implementations\n";
    run_interface_demo(std::make_unique<LinkedGraph>(), "LinkedGraph");
    run_interface_demo(std::make_unique<MatrixGraph>(), "MatrixGraph");
}

void demo_pro24() {
    MatrixGraph graph;
    graph.insert_edge("X", "m0", "Y");
    graph.insert_edge("X", "m1", "Y");
    graph.insert_edge("Y", "m2", "Z");
    graph.insert_edge("Z", "m3", "X");

    std::cout << "Problem 2.4 demo: matrix-based graph implementation\n";
    std::cout << "This demo uses MatrixGraph with multiple labels between the same nodes.\n";
    print_graph_summary(graph);
}

void demo_pro25() {
    LinkedGraph graph;
    graph.insert_edge("A", "x", "B");
    graph.insert_edge("A", "y", "B");
    graph.insert_edge("B", "z", "C");
    graph.insert_edge("D", "self", "D");

    std::cout << "Problem 2.5 demo: deleting edges and nodes with isolated-node cleanup\n";
    std::cout << "Initial graph:\n";
    print_graph_summary(graph);

    graph.disconnect("A", "B");
    std::cout << "\nAfter disconnect(\"A\", \"B\"):\n";
    print_graph_summary(graph);

    graph.remove_node("B");
    std::cout << "\nAfter remove_node(\"B\"):\n";
    print_graph_summary(graph);
}

template <typename GraphT>
void demonstrate_rule_of_five(const std::string& name) {
    GraphT original;
    original.insert_edge("A", "r0", "B");
    original.insert_edge("B", "r1", "C");

    GraphT copy_constructed(original);
    original.insert_edge("C", "late", "D");

    GraphT copy_assigned;
    copy_assigned = original;

    GraphT move_constructed(std::move(copy_constructed));

    GraphT move_assigned;
    move_assigned = std::move(copy_assigned);

    std::cout << name << ":\n";
    std::cout << "  original edges after local mutation: " << original.edges().size() << '\n';
    std::cout << "  moved-from copy-constructed graph transferred to new object with edges: "
              << move_constructed.edges().size() << '\n';
    std::cout << "  moved-from copy-assigned graph transferred to new object with edges: "
              << move_assigned.edges().size() << '\n';
}

void demo_pro26() {
    std::cout << "Problem 2.6 demo: rule of five\n";
    std::cout << "Copy operations preserve graph contents, and move operations transfer ownership.\n";
    demonstrate_rule_of_five<LinkedGraph>("LinkedGraph");
    demonstrate_rule_of_five<MatrixGraph>("MatrixGraph");
}

}  // namespace

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: ./graph-demo-pro2 <pro21|pro22|pro23|pro24|pro25|pro26>\n";
        return 1;
    }

    const std::string mode = argv[1];
    if (mode == "pro21") {
        demo_pro21();
        return 0;
    }
    if (mode == "pro22") {
        demo_pro22();
        return 0;
    }
    if (mode == "pro23") {
        demo_pro23();
        return 0;
    }
    if (mode == "pro24") {
        demo_pro24();
        return 0;
    }
    if (mode == "pro25") {
        demo_pro25();
        return 0;
    }
    if (mode == "pro26") {
        demo_pro26();
        return 0;
    }

    std::cerr << "Unknown demo target: " << mode << '\n';
    return 1;
}
