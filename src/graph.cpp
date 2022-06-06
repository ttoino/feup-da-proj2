#include <queue>
#include <list>
#include <algorithm>
#include <sstream>

#include "../includes/constants.hpp"
#include "../includes/graph.hpp"
#include "../includes/utils.hpp"

// Add edge from source to destination with a certain weight
void Graph::addEdge(Node &src, Node &dest, const int capacity,
                      const int duration) {
    src.adj.insert({dest.label, {dest.label, capacity, duration}});
}

// Add edge from source to destination with a certain weight
void Graph::addEdge(int src, int dest, int capacity, int duration) {
    addEdge(nodes[src], nodes[dest], capacity, duration);
}

void Graph::bfs(int s, int t) {
    this->resetVisits();

    // queue of unvisited nodes with distance to s
    std::queue<std::pair<int, int>> q;

    q.push({s, 0});
    nodes[s].visited = true;
    nodes[s].parent = s;
    int nStops = -1;

    while (!q.empty()) { // while there are still unvisited nodes
        auto [currentNode, dist] = q.front();
        q.pop();

        auto &node = nodes[currentNode];

        for (const auto &[dest, e] : node.adj) {
            if (!nodes[dest].visited) {
                q.push({dest, dist + 1});
                nodes[dest].visited = true;
                nodes[dest].parent = currentNode;

                if (dest == t)
                    goto bfs_exitwhile;
            }
        }
    }
bfs_exitwhile: return;
}

int Graph::edmondsKarpBFS(int s, int t) {
    resetVisits();

    nodes.at(s).parent = s;
    nodes.at(s).visited = true;
    std::queue<std::pair<int, int>> q;
    q.push({s, INT_MAX});

    while (!q.empty()) {
        auto [cur, flow] = q.front();
        q.pop();

        for (auto &[dest, next] : nodes.at(cur).adj) {
            if (!nodes.at(dest).visited && residualGraph.at(cur).at(dest) > 0) {
                nodes.at(dest).parent = cur;
                int new_flow = std::min(flow, residualGraph.at(cur).at(dest));

                if (dest == t)
                    return new_flow;

                q.push({dest, new_flow});
            }
        }
    }

    return -1;
}

std::pair<int, Graph> Graph::edmondsKarp(int start, int end, int groupSize) {
    Graph graph{};

    int flow = 0, new_flow = 0;

    for (auto &[src, node] : nodes)
        for (auto &[dest, e] : node.adj)
            residualGraph.at(src).at(dest) = e.capacity;

    while (flow < groupSize) {
        new_flow = edmondsKarpBFS(start, end);

        if (new_flow == -1)
            break;

        // we found a new valid augment path, update path graph

        int currentNode = nodes.size();
        do {
            int parentNode = nodes.at(currentNode).parent;
            auto &edge = nodes.at(parentNode).adj.at(currentNode);

            if (!graph.hasNode(currentNode))
                graph.addNode(currentNode);

            if (!graph.hasNode(parentNode))
                graph.addNode(parentNode);

            graph.addEdge(parentNode, currentNode, edge.capacity,
                          edge.duration);

            currentNode = parentNode;
        } while (currentNode != start);

        // path = parent;
        flow += new_flow;
        int cur = end;

        while (cur != start) {
            int prev = nodes.at(cur).parent;
            residualGraph.at(prev).at(cur) -= new_flow;
            residualGraph.at(cur).at(prev) += new_flow;
            cur = prev;
        }
    }

    return {flow, graph};
}

void Graph::resetVisits() {
    for (auto& [index, node] : nodes) {
        node.visited = false;
        node.parent = -1;
    }
}

void Graph::addNode(int i) {
    this->nodes.insert({i, {i}});
}

void Graph::addNode(int i, const Node& node) {
    this->nodes.insert({i, node});
}
