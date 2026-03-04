#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>

#include "Graph.h"
#include "Node.h"
#include "adj_matrix.h"
#include "edge.h"

namespace {

struct CheckContext {
    int passed = 0;
    int failed = 0;

    void expect(bool condition, const std::string& message) {
        if (condition) {
            ++passed;
            std::cout << "  [PASS] " << message << "\n";
        } else {
            ++failed;
            std::cout << "  [FAIL] " << message << "\n";
        }
    }

    bool ok() const {
        return failed == 0;
    }
};

void printHeader(const std::string& title) {
    std::cout << "\n==================================================\n";
    std::cout << title << "\n";
    std::cout << "==================================================\n";
}

bool finishSection(const std::string& sectionTitle, const CheckContext& checks) {
    std::cout << "Summary: " << checks.passed << " passed, " << checks.failed << " failed.\n";
    std::cout << sectionTitle << ": " << (checks.ok() ? "SATISFIED" : "NOT SATISFIED") << "\n";
    return checks.ok();
}

void insertCommonSample(AbstractGraph& graph) {
    graph.insert_edge("Paris", "road", "London");
    graph.insert_edge("London", "train", "Berlin");
    graph.insert_edge("Berlin", "flight", "Paris");
    graph.insert_edge("Paris", "flight", "Paris");
    graph.insert_edge("Paris", "bus", "London");
}

int countEdges(const AbstractGraph& graph, const std::string& source, const std::string& target) {
    int count = 0;
    for (Edge* edge : graph.edges()) {
        if (edge->sourceNode()->label() == source && edge->targetNode()->label() == target) {
            ++count;
        }
    }
    return count;
}

bool test21_graph_incidence_lists() {
    printHeader("2.1 Graph Data Structure Based On Incidence Lists");
    CheckContext checks;

    Graph graph;
    insertCommonSample(graph);

    checks.expect(graph.nodes().size() == 3, "Node labels are unique after inserts");
    checks.expect(graph.edges().size() == 5, "All inserted edges are stored (including parallel + self-loop)");
    checks.expect(graph.find_node("Paris") != nullptr, "find_node can find existing node");
    checks.expect(graph.find_node("Tokyo") == nullptr, "find_node returns null for missing node");
    checks.expect(countEdges(graph, "Paris", "London") == 2, "Parallel edges between same nodes are allowed");

    Node* paris = graph.find_node("Paris");
    checks.expect(paris != nullptr, "Paris exists");
    if (paris != nullptr) {
        checks.expect(paris->incidentEdges().size() == 4, "Node keeps incidence list of connected edges");
    }

    if (!graph.edges().empty()) {
        checks.expect(graph.edges().front()->incidentNodes().size() == 2,
                      "Edge keeps incidence list of source/target nodes");
    } else {
        checks.expect(false, "Graph has at least one edge");
    }

    std::set<std::string> uniqueLabels;
    for (Node* node : graph.nodes()) {
        uniqueLabels.insert(node->label());
    }
    checks.expect(uniqueLabels.size() == graph.nodes().size(), "Node labels are unique");

    return finishSection("2.1", checks);
}

bool lineHasRequiredDotFormat(const std::string& line) {
    if (line.empty()) {
        return false;
    }

    if (line.back() != '.') {
        return false;
    }

    std::string withoutDot = line.substr(0, line.size() - 1);
    std::istringstream stream(withoutDot);
    std::string a;
    std::string b;
    std::string c;
    std::string extra;

    if (!(stream >> a >> b >> c)) {
        return false;
    }

    return !(stream >> extra);
}

bool test22_file_io() {
    printHeader("2.2 File Input/Output");
    CheckContext checks;

    Graph graph;
    insertCommonSample(graph);
    graph.save("incidence_graph.txt");

    std::ifstream file("incidence_graph.txt");
    checks.expect(file.good(), "Graph::save created output file");
    std::string firstLine;
    std::getline(file, firstLine);
    checks.expect(!firstLine.empty(), "Output file is not empty");
    checks.expect(lineHasRequiredDotFormat(firstLine),
                  "Saved line format matches: node_a edge_label node_b.");

    Graph loadedGraph;
    loadedGraph.load("incidence_graph.txt");
    checks.expect(loadedGraph.nodes().size() == graph.nodes().size(), "load restores node count");
    checks.expect(loadedGraph.edges().size() == graph.edges().size(), "load restores edge count");

    return finishSection("2.2", checks);
}

void runInterfaceSmoke(AbstractGraph& graph, CheckContext& checks) {
    graph.insert_edge("A", "e1", "B");
    graph.insert_edge("A", "e2", "B");
    graph.insert_edge("B", "e3", "C");
    checks.expect(graph.nodes().size() == 3, "Abstract interface supports node insertion");
    checks.expect(graph.edges().size() == 3, "Abstract interface supports edge insertion");
}

bool test23_abstract_interface() {
    printHeader("2.3 Abstract Class As Interface");
    CheckContext checks;

    Graph incidenceGraph;
    MatrixGraph matrixGraph;

    AbstractGraph& graphAsInterface = incidenceGraph;
    AbstractGraph& matrixAsInterface = matrixGraph;

    runInterfaceSmoke(graphAsInterface, checks);
    runInterfaceSmoke(matrixAsInterface, checks);

    return finishSection("2.3", checks);
}

bool test24_matrix_graph() {
    printHeader("2.4 Matrix-Based Data Structure");
    CheckContext checks;

    MatrixGraph graph;
    graph.insert_edge("A", "ab1", "B");
    graph.insert_edge("A", "ab2", "B");
    graph.insert_edge("B", "ba1", "A");
    graph.insert_edge("A", "loop", "A");

    checks.expect(graph.nodes().size() == 2, "Matrix graph keeps unique node objects");
    checks.expect(graph.edges().size() == 4, "Matrix graph stores multiple edge labels per cell");
    checks.expect(countEdges(graph, "A", "B") == 2, "Multiple edges between same pair are kept");
    checks.expect(countEdges(graph, "B", "A") == 1, "Directed asymmetry is preserved");
    checks.expect(countEdges(graph, "A", "A") == 1, "Self-loop is supported");

    return finishSection("2.4", checks);
}

void runDeleteScenario(AbstractGraph& graph, CheckContext& checks) {
    graph.insert_edge("A", "e1", "B");
    graph.insert_edge("A", "e2", "B");
    graph.insert_edge("B", "e3", "C");
    graph.insert_edge("C", "e4", "A");
    graph.insert_edge("D", "e5", "A");

    checks.expect(graph.nodes().size() == 4 && graph.edges().size() == 5, "Setup for deletion scenario is correct");

    graph.disconnect("A", "B");
    checks.expect(graph.edges().size() == 3, "disconnect removes all edges from source to target");
    checks.expect(graph.nodes().size() == 4, "disconnect keeps non-isolated nodes");

    graph.remove_node("C");
    checks.expect(graph.edges().size() == 1, "remove_node removes all incident edges");
    checks.expect(graph.nodes().size() == 2, "remove_node also removes isolated nodes created by deletion");

    graph.disconnect("D", "A");
    checks.expect(graph.nodes().empty() && graph.edges().empty(),
                  "disconnect cleans up final isolated nodes");
}

bool test25_delete_edges_and_nodes() {
    printHeader("2.5 Delete Edges And Nodes");
    CheckContext checks;

    Graph incidenceGraph;
    MatrixGraph matrixGraph;

    runDeleteScenario(incidenceGraph, checks);
    runDeleteScenario(matrixGraph, checks);

    return finishSection("2.5", checks);
}

template <typename GraphType>
void runRuleOfFiveChecks(const std::string& typeName, CheckContext& checks) {
    checks.expect(std::is_copy_constructible<GraphType>::value, typeName + " is copy constructible");
    checks.expect(std::is_copy_assignable<GraphType>::value, typeName + " is copy assignable");
    checks.expect(std::is_move_constructible<GraphType>::value, typeName + " is move constructible");
    checks.expect(std::is_move_assignable<GraphType>::value, typeName + " is move assignable");

    auto* original = new GraphType();
    original->insert_edge("A", "e1", "B");
    auto* copied = new GraphType(*original);

    bool deepCopyNodes = !original->nodes().empty() &&
                         !copied->nodes().empty() &&
                         original->nodes().front() != copied->nodes().front();
    checks.expect(deepCopyNodes, typeName + " copy constructor performs deep copy");

    GraphType moveSource;
    moveSource.insert_edge("X", "m1", "Y");
    auto* moved = new GraphType(std::move(moveSource));
    (void)moved;

    bool movedFromIsEmpty = moveSource.nodes().empty() && moveSource.edges().empty();
    checks.expect(movedFromIsEmpty, typeName + " move constructor leaves source empty");

    // Intentional leaks: these types currently use shallow copy/move semantics and would
    // double-delete on destruction after copy/move in current implementation.
    (void)original;
    (void)copied;
}

bool test26_rule_of_five() {
    printHeader("2.6 Rule Of Five: Copying And Moving");
    CheckContext checks;

    runRuleOfFiveChecks<Graph>("Graph", checks);
    runRuleOfFiveChecks<MatrixGraph>("MatrixGraph", checks);

    return finishSection("2.6", checks);
}

}

int main() {
    const bool req21 = test21_graph_incidence_lists();
    const bool req22 = test22_file_io();
    const bool req23 = test23_abstract_interface();
    const bool req24 = test24_matrix_graph();
    const bool req25 = test25_delete_edges_and_nodes();
    const bool req26 = test26_rule_of_five();

    const bool allRequirementsSatisfied = req21 && req22 && req23 && req24 && req25 && req26;

    std::cout << "\n==================================================\n";
    std::cout << "Final Requirement Summary\n";
    std::cout << "==================================================\n";
    std::cout << "2.1: " << (req21 ? "SATISFIED" : "NOT SATISFIED") << "\n";
    std::cout << "2.2: " << (req22 ? "SATISFIED" : "NOT SATISFIED") << "\n";
    std::cout << "2.3: " << (req23 ? "SATISFIED" : "NOT SATISFIED") << "\n";
    std::cout << "2.4: " << (req24 ? "SATISFIED" : "NOT SATISFIED") << "\n";
    std::cout << "2.5: " << (req25 ? "SATISFIED" : "NOT SATISFIED") << "\n";
    std::cout << "2.6: " << (req26 ? "SATISFIED" : "NOT SATISFIED") << "\n";

    if (allRequirementsSatisfied) {
        std::cout << "All requirements are satisfied.\n";
    } else {
        std::cout << "Not all requirements are satisfied.\n";
    }

    return 0;
}
