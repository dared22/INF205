#include "Algorithms.h"

#include "AbstractGraph.h"
#include "Edge.h"
#include "Node.h"

#include <algorithm>
#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

namespace {

using AdjacencyList = std::unordered_map<const Node*, std::vector<const Edge*>>;

AdjacencyList build_outgoing_edges(const AbstractGraph& graph) {
    AdjacencyList outgoing;
    outgoing.reserve(graph.nodes().size());

    for (const Node* node : graph.nodes()) {
        outgoing[node];
    }

    for (const Edge* edge : graph.edges()) {
        outgoing[edge->sourceNode()].push_back(edge);
    }

    return outgoing;
}

struct TarjanState {
    std::unordered_map<const Node*, int> index_by_node;
    std::unordered_map<const Node*, int> lowlink_by_node;
    std::unordered_map<const Node*, bool> on_stack;
    std::vector<const Node*> stack;
    std::vector<std::vector<const Node*>> components;
    int next_index = 0;
};

void strongconnect(const Node* node, const AdjacencyList& outgoing_edges, TarjanState& state) {
    state.index_by_node[node] = state.next_index;
    state.lowlink_by_node[node] = state.next_index;
    ++state.next_index;

    state.stack.push_back(node);
    state.on_stack[node] = true;

    const auto outgoing = outgoing_edges.find(node);
    if (outgoing != outgoing_edges.end()) {
        for (const Edge* edge : outgoing->second) {
            const Node* successor = edge->targetNode();
            if (state.index_by_node.find(successor) == state.index_by_node.end()) {
                strongconnect(successor, outgoing_edges, state);
                state.lowlink_by_node[node] =
                    std::min(state.lowlink_by_node[node], state.lowlink_by_node[successor]);
            } else if (state.on_stack[successor]) {
                state.lowlink_by_node[node] =
                    std::min(state.lowlink_by_node[node], state.index_by_node[successor]);
            }
        }
    }

    if (state.lowlink_by_node[node] != state.index_by_node[node]) {
        return;
    }

    std::vector<const Node*> component;
    while (!state.stack.empty()) {
        const Node* member = state.stack.back();
        state.stack.pop_back();
        state.on_stack[member] = false;
        component.push_back(member);
        if (member == node) {
            break;
        }
    }

    std::sort(component.begin(), component.end(), [](const Node* left, const Node* right) {
        return left->label() < right->label();
    });
    state.components.push_back(component);
}

std::vector<std::vector<std::size_t>> compute_sequence_reachability(
    const AbstractGraph& graph,
    const std::vector<std::string>& sequence
) {
    const auto& nodes = graph.nodes();
    const std::size_t node_count = nodes.size();
    const AdjacencyList outgoing_edges = build_outgoing_edges(graph);

    std::unordered_map<const Node*, std::size_t> node_index;
    node_index.reserve(node_count);
    for (std::size_t index = 0; index < node_count; ++index) {
        node_index[nodes[index]] = index;
    }

    std::vector<std::vector<std::size_t>> targets_by_start(node_count);
    if (sequence.empty()) {
        for (std::size_t start = 0; start < node_count; ++start) {
            targets_by_start[start].push_back(start);
        }
        return targets_by_start;
    }

    std::vector<int> seen(node_count, -1);
    int stamp = 0;

    for (std::size_t start = 0; start < node_count; ++start) {
        std::vector<std::size_t> frontier(1, start);

        for (const std::string& label : sequence) {
            std::vector<std::size_t> next_frontier;
            ++stamp;

            for (const std::size_t current_index : frontier) {
                const Node* current = nodes[current_index];
                const auto outgoing = outgoing_edges.find(current);
                if (outgoing == outgoing_edges.end()) {
                    continue;
                }

                for (const Edge* edge : outgoing->second) {
                    if (edge->label() != label) {
                        continue;
                    }

                    const std::size_t target_index = node_index[edge->targetNode()];
                    if (seen[target_index] == stamp) {
                        continue;
                    }

                    seen[target_index] = stamp;
                    next_frontier.push_back(target_index);
                }
            }

            frontier = next_frontier;
            if (frontier.empty()) {
                break;
            }
        }

        std::sort(frontier.begin(), frontier.end());
        frontier.erase(std::unique(frontier.begin(), frontier.end()), frontier.end());
        targets_by_start[start] = frontier;
    }

    return targets_by_start;
}

}  // namespace

std::vector<std::vector<const Node*>> tarjan_strongly_connected_components(const AbstractGraph& graph) {
    const auto& nodes = graph.nodes();
    const AdjacencyList outgoing_edges = build_outgoing_edges(graph);
    TarjanState state;

    for (const Node* node : nodes) {
        if (state.index_by_node.find(node) == state.index_by_node.end()) {
            strongconnect(node, outgoing_edges, state);
        }
    }

    std::sort(
        state.components.begin(),
        state.components.end(),
        [](const std::vector<const Node*>& left, const std::vector<const Node*>& right) {
            if (left.empty() || right.empty()) {
                return left.size() < right.size();
            }
            return left.front()->label() < right.front()->label();
        }
    );

    return state.components;
}

std::vector<std::pair<const Node*, const Node*>> find_diamond_pairs(
    const AbstractGraph& graph,
    const std::vector<std::string>& first_sequence,
    const std::vector<std::string>& second_sequence
) {
    const auto& nodes = graph.nodes();
    if (nodes.empty()) {
        return {};
    }

    const std::vector<std::vector<std::size_t>> first_targets =
        compute_sequence_reachability(graph, first_sequence);
    const std::vector<std::vector<std::size_t>> second_targets =
        compute_sequence_reachability(graph, second_sequence);

    const std::size_t node_count = nodes.size();
    std::vector<int> in_second(node_count, -1);
    int stamp = 0;

    std::vector<std::pair<const Node*, const Node*>> pairs;
    for (std::size_t start = 0; start < node_count; ++start) {
        ++stamp;
        for (const std::size_t target : second_targets[start]) {
            in_second[target] = stamp;
        }

        for (const std::size_t target : first_targets[start]) {
            if (in_second[target] == stamp) {
                pairs.emplace_back(nodes[start], nodes[target]);
            }
        }
    }

    std::sort(
        pairs.begin(),
        pairs.end(),
        [](const std::pair<const Node*, const Node*>& left,
           const std::pair<const Node*, const Node*>& right) {
            if (left.first->label() != right.first->label()) {
                return left.first->label() < right.first->label();
            }
            return left.second->label() < right.second->label();
        }
    );

    return pairs;
}
