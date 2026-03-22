#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <string>
#include <utility>
#include <vector>

class AbstractGraph;
class Node;

std::vector<std::vector<const Node*>> tarjan_strongly_connected_components(const AbstractGraph& graph);

std::vector<std::pair<const Node*, const Node*>> find_diamond_pairs(
    const AbstractGraph& graph,
    const std::vector<std::string>& first_sequence,
    const std::vector<std::string>& second_sequence
);

#endif
