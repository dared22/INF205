#ifndef MATRIXGRAPH_H
#define MATRIXGRAPH_H

#include "abstract_graph.h"
#include "Node.h"

#include <string>
#include <vector>
#include <map>

class MatrixGraph : public AbstractGraph {
private:

    std::vector<Node*> nodes_;

    std::map<std::string, int> labelToIndex_;

    std::vector<std::vector<std::vector<std::string>>> matrix_;

    std::vector<Edge*> edges_;

    int findOrCreateNodeIndex(const std::string& label);

    void expandMatrix();
    void removeNodeByIndex(int index);
    void removeIfIsolated(int index);
    bool hasConnections(int index);

public:
    MatrixGraph() = default;                             // default constructor
    ~MatrixGraph() override;

    // Rule of Five task 2.6 (destructor already implemented before this task)
    MatrixGraph(const MatrixGraph& other);               // copy constructor
    MatrixGraph& operator=(const MatrixGraph& other);    // copy assignment
    MatrixGraph(MatrixGraph&& other) noexcept;           // move constructor
    MatrixGraph& operator=(MatrixGraph&& other) noexcept;// move assignment

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
