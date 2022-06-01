#include <algorithm>
#include <filesystem>
#include <fstream>
#include <queue>
#include <list>

#include "../includes/constants.hpp"
#include "../includes/dataset.hpp"
#include "../includes/utils.hpp"

Dataset::Dataset(const unsigned int n) {
    for (int i = 1; i <= n; ++i)
        nodes.insert({i, {i}});
    residualGraph.resize(nodes.size() + 1, std::vector<int>(nodes.size() + 1));
}

Dataset::Dataset() {}

Dataset Dataset::load(const std::string &path) {

    if (path == "output.csv") return {};

    std::ifstream dataset_file{DATASETS_PATH + path};

    if (!dataset_file.is_open())
        return {};

    std::string line;
    std::vector<std::string> tokens;

    std::getline(dataset_file, line);

    if (line.empty()) return {};

    tokens = split(line, ' ');

    unsigned int n = stoul(tokens[0]);
    unsigned int t = stoul(tokens[1]);

    Dataset result{n};

    for (int i = 0; i < t; ++i) {
        std::getline(dataset_file, line);
        tokens = split(line, ' ');

        result.addEdge(stoul(tokens[0]), stoul(tokens[1]), stoul(tokens[2]),
                       stoul(tokens[3]));
    }

    return result;
}

Dataset Dataset::generate(const std::string &name,
                          const DatasetGenerationParams &params) {
    // TODO
    std::filesystem::create_directory({DATASETS_PATH + name});

    return {};
}

std::vector<std::string> Dataset::getAvailableDatasets() {
    std::vector<std::string> result{};

    for (auto &p : std::filesystem::directory_iterator(DATASETS_PATH))
        if (p.is_regular_file() && p.path().filename().string().find("output") == std::string::npos)
            result.push_back(p.path().filename().string());

    std::sort(result.begin(), result.end());

    return result;
}

// Add edge from source to destination with a certain weight
void Dataset::addEdge(Node &src, Node &dest, const int capacity,
                      const int duration) {
    src.adj.insert({dest.label, capacity, duration});
}

// Add edge from source to destination with a certain weight
void Dataset::addEdge(const int src, const int &dest, const int capacity,
                      const int duration) {
    addEdge(nodes[src], nodes[dest], capacity, duration);
    residualGraph.at(src).at(dest) = capacity;
}

int Dataset::EK_bfs(
    int s, int t, std::vector<int>* parent,
    std::vector<std::vector<int>>* residualGraph
    ) {

    for (unsigned i = 1; i <= nodes.size(); i++) 
        parent->at(i) = -1;

    parent->at(s) = -2;         
    std::queue<std::pair<int, int>> q;
    q.push({s, INT_MAX});

    while (!q.empty()) {

        int cur = q.front().first;
        int flow = q.front().second;
        q.pop();

        for (Edge next : nodes.at(cur).adj) {
            int dest = next.dest;

            if (parent->at(dest) == -1 && residualGraph->at(cur).at(dest) > 0) {
                parent->at(dest) = cur;
                int new_flow = std::min(flow, residualGraph->at(cur).at(dest));

                if (dest == t) {
                    return new_flow;
                }
                    
                q.push({dest, new_flow});
            }
        }
    }

  return 0;
}

std::pair<int, std::vector<int>> Dataset::edmondsKarp(
    int s, int t, EdmondsKarpUsage usage, int groupSize) {
    
    int flow = 0, new_flow = 0;
    std::vector<int> parent(nodes.size() + 1);
    std::vector<int> path;
    std::vector<std::vector<int>> residualGraph = this->getCap();

    if(usage == EdmondsKarpUsage::DEFAULT) {
        while (true) {
            new_flow = EK_bfs(s, t, &parent, &residualGraph);
            
            if (new_flow == 0) 
                break;            

            path = parent;
            flow += new_flow;
            int cur = t;

            while (cur != s) {
                int prev = parent.at(cur);	
                residualGraph.at(prev).at(cur) -= new_flow;
                residualGraph.at(cur).at(prev) += new_flow;
                cur = prev;
                
            }
        }
    } else if (usage == EdmondsKarpUsage::CUSTOM) {
        while (flow < groupSize) {
            new_flow = EK_bfs(s, t, &parent, &residualGraph);
            
            if (new_flow == 0) 
                break;            

            path = parent;
            flow += new_flow;
            int cur = t;

            while (cur != s) {
                int prev = parent.at(cur);	
                residualGraph.at(prev).at(cur) -= new_flow;
                residualGraph.at(cur).at(prev) += new_flow;
                cur = prev;
                
            }
        }        
    }

    return { flow, path };
}

std::pair<int, std::vector<int>> Dataset::BFS(int s, int t) {

    std::list<int> path_list;

    this->visitedFalse();

    std::queue<std::pair<int ,int>> q; // queue of unvisited nodes with distance to s
    q.push({s, 0});
    nodes[s].visited = true;
    nodes[s].parent = s;
    int nStops = -1;
    while (!q.empty()) { // while there are still unvisited nodes
        int u = q.front().first;
        int u1 = q.front().second; q.pop();

        auto& node = nodes[u];

        bool toBreak = false;

        for (const auto& e : node.adj) {

            int w = e.dest;

            if (!nodes[w].visited) {
                q.push({w, u1 + 1});
                nodes[w].visited = true;
                nodes[w].parent = u;

                if (w == t) {
                    toBreak = true;
                    break;
                }
            }
        }

        if (toBreak) break;
    }

    int cur = t;
    int maxCap = INT_MIN;

    while (cur != s) {
        int prev = nodes[cur].parent;
        path_list.emplace_front(cur);

        auto prevNodeEdges = this->getNode(prev).adj;

        int edgeCap = std::find_if(prevNodeEdges.begin(), prevNodeEdges.end(), [&cur](const Edge& e) -> bool { return e.dest == cur; })->capacity;

        if (edgeCap > maxCap) maxCap = edgeCap;

        cur = prev;
    }
    path_list.emplace_front(s);

    auto path = std::vector<int>(path_list.begin(), path_list.end());

    return {maxCap, path};
}

void Dataset::visitedFalse() {
    for (auto& [index, node] : nodes) {
        node.visited = false;
        node.parent = -1;
    }
}