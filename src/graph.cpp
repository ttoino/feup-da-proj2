#include <algorithm>
#include <fstream>
#include <list>
#include <map>
#include <queue>
#include <sstream>
#include <unordered_map>

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
bfs_exitwhile:
    return;
}

int Graph::edmondsKarpBFS(int s, int t) {
    resetVisits();

    std::queue<std::pair<int, int>> q;

    q.push({s, INT_MAX});
    nodes.at(s).parent = s;
    nodes.at(s).visited = true;

    while (!q.empty()) {
        auto [cur, flow] = q.front();
        q.pop();

        auto &node = nodes.at(cur);

        for (const auto &[dest, next] : node.adj) {
            if (!nodes.at(dest).visited && residualGraph.at(cur).at(dest) > 0) {
                nodes.at(dest).parent = cur;
                nodes.at(dest).visited = true;
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
    Graph graph{nodes.size()};

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
    for (auto &[index, node] : nodes) {
        node.visited = false;
        node.parent = -1;
    }
}

void Graph::addNode(int i) { this->nodes.insert({i, {i}}); }

void Graph::addNode(int i, const Node &node) { this->nodes.insert({i, node}); }

void Graph::toDotFile(const std::string &path,
                      const std::vector<std::pair<Graph, std::string>> paths) {
    static const std::unordered_map<int, std::string> colors{
        {0, "black"}, {1, "red"},    {2, "blue"}, {3, "purple"},
        {4, "green"}, {5, "yellow"}, {6, "cyan"}, {7, "gray"}};

    std::ofstream out{path};

    out << DOT_HEADER;

    if (nodes.size() < 100)
        out << "splines=true\n";

    if (!paths.empty()) {
        out << "{ Legend [shape=none, margin=0, label=<"
               "<table border=\"0\" cellborder=\"1\" cellspacing=\"0\" "
               "cellpadding=\"4\">";

        for (int i = 0; i < paths.size(); ++i)
            out << "<tr>"
                   "<td bgcolor=\""
                << colors.at(1 << i) << "\"></td><td>" << i + 1 << ". "
                << paths.at(i).second
                << "</td>"
                   "</tr>";

        for (int i = 1; i < 1 << paths.size(); ++i) {
            if ((i & (i - 1)) == 0)
                continue;

            bool first = true;

            out << "<tr>"
                   "<td bgcolor=\""
                << colors.at(i) << "\"></td><td>";

            for (int j = 0; j < paths.size(); ++j) {
                if (i & (1 << j)) {
                    if (!first)
                        out << " + ";

                    out << j + 1;
                    first = false;
                }
            }

            out << "</td>"
                   "</tr>";
        }

        out << "</table>"
               ">]}\n";
    }

    std::map<std::pair<int, int>, int> edges{};

    for (auto &[src, node] : nodes)
        for (auto &[dest, edge] : node.adj)
            edges[{src, dest}] = 0;

    for (int i = 0; i < paths.size(); ++i)
        for (auto &[src, node] : paths.at(i).first.nodes)
            for (auto &[dest, edge] : node.adj)
                edges[{src, dest}] |= 1 << i;

    for (auto &[p, color] : edges)
        out << p.first << " -> " << p.second << " [color=" << colors.at(color)
            << "]\n";

    out << "}\n";
    out.close();
}
