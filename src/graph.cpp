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
    src.adj.insert({dest.label, capacity, duration});
}

// Add edge from source to destination with a certain weight
void Graph::addEdge(int src, int dest, int capacity, int duration) {
    addEdge(nodes[src], nodes[dest], capacity, duration);
}

std::pair<int, std::list<int>> Graph::BFS(int s, int t) {

    this->resetVisits();

    std::list<int> path_list;
    std::queue<std::pair<int ,int>> q; // queue of unvisited nodes with distance to s
    
    q.push({s, 0});
    nodes[s].visited = true;
    nodes[s].parent = s;
    int nStops = -1;

    while (!q.empty()) { // while there are still unvisited nodes
        int currentNode = q.front().first;
        int currentNodeDistToSrc = q.front().second; q.pop();

        auto& node = nodes[currentNode];

        bool toBreak = false;

        for (const auto& e : node.adj) {

            int edgeDest = e.dest;

            if (!nodes[edgeDest].visited) {
                q.push({edgeDest, currentNodeDistToSrc + 1});
                nodes[edgeDest].visited = true;
                nodes[edgeDest].parent = currentNode;

                if (edgeDest == t) {
                    toBreak = true;
                    break;
                }
            }
        }

        if (toBreak) break;
    }

    int cur = t;
    int minCap = INT_MAX;

    while (cur != s) {
        int prev = nodes[cur].parent;
        path_list.emplace_front(cur);

        auto prevNodeEdges = this->getNode(prev).adj;

        int edgeCap = std::find_if(prevNodeEdges.begin(), prevNodeEdges.end(), [&cur](const Edge& e) -> bool { return e.dest == cur; })->capacity;

        if (edgeCap < minCap) minCap = edgeCap;

        cur = prev;
    }
    path_list.emplace_front(s);

    return {minCap, path_list};
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

std::string Graph::toDotFile() {
    std::stringstream out{};

    out << "digraph {\noverlap=scale\nsplines=true\nnode [shape=circle]\n";

    for (auto p : nodes)
        for (auto e : p.second.adj)
            out << p.first << " -> " << e.dest << '\n';

    out << "}\n";

    return out.str();
}
