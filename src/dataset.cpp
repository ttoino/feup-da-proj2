#include <algorithm>
#include <filesystem>
#include <fstream>
#include <queue>
#include <list>

#include "../includes/constants.hpp"
#include "../includes/dataset.hpp"
#include "../includes/utils.hpp"

Dataset::Dataset(const Graph& graph): network(graph) {}

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

    Graph result{n};

    std::vector<std::vector<int>> residualGraphCopy(n + 1, std::vector<int>(n + 1));

    for (int i = 0; i < t; ++i) {
        std::getline(dataset_file, line);
        tokens = split(line, ' ');

        int src = stoul(tokens[0]), dest = stoul(tokens[1]), capacity = stoul(tokens[2]), duration = stoul(tokens[3]);

        result.addEdge(src, dest, capacity, duration);
        residualGraphCopy.at(src).at(dest) = capacity;
    }

    Dataset dataset{result};
    dataset.residualGraph = residualGraphCopy;
    return dataset;
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

std::pair<int, std::list<int>> Dataset::EK_bfs(
    int s, int t, std::vector<int>& parent,
    std::vector<std::vector<int>>& residualGraph
    ) {

    std::list<int> path;

    auto& nodes = this->network.getNodes();

    for (unsigned i = 1; i <= nodes.size(); i++) 
        parent.at(i) = -1;

    parent.at(s) = s;         
    std::queue<std::pair<int, int>> q;
    q.push({s, INT_MAX});

    while (!q.empty()) {

        int cur = q.front().first;
        int flow = q.front().second;
        q.pop();

        for (Edge next : nodes.at(cur).adj) {
            int dest = next.dest;

            if (parent.at(dest) == -1 && residualGraph.at(cur).at(dest) > 0) {
                parent.at(dest) = cur;
                path.emplace_back(dest);
                int new_flow = std::min(flow, residualGraph.at(cur).at(dest));

                if (dest == t) {
                    return {new_flow, path};
                }
                    
                q.push({dest, new_flow});
            }
        }
    }

  return { 0, std::list<int>() };
}

std::pair<int, std::vector<std::list<int>>> Dataset::edmondsKarp(
    int s, int t, EdmondsKarpUsage usage, int groupSize) {
    
    this->path.clear(); // ensure old paths don't mess-up this intance of calculations

    auto& nodes = this->network.getNodes();

    int flow = 0, new_flow = 0;
    std::vector<int> parent(nodes.size() + 1);
    std::list<int> bfs_path;
    std::vector<std::list<int>> paths;
    std::pair<int, std::list<int>> bfsResult;
    std::vector<std::vector<int>> residualGraph = this->getCap();

    if(usage == EdmondsKarpUsage::DEFAULT) {
        while (true) {
            bfsResult = EK_bfs(s, t, parent, residualGraph);
            new_flow = bfsResult.first;
            bfs_path = bfsResult.second;
            
            if (new_flow == 0) 
                break;            

            // we found a new valid augment path, update path graph

            int currentNode = nodes.size();
            do {
                int parentNode = parent.at(currentNode);

                auto startNode = this->network.getNode(parentNode);
                auto destNode = this->network.getNode(currentNode);

                auto startNodeEdges = startNode.adj;

                auto edge = std::find_if(startNodeEdges.begin(), startNodeEdges.end(), [&destNode](const Edge& edge) -> bool { return edge.dest == destNode.label; });

                if (edge == startNodeEdges.end()) continue;

                if (!this->path.hasNode(startNode.label))
                    this->path.addNode(startNode.label);

                if (!this->path.hasNode(destNode.label))
                    this->path.addNode(destNode.label);

                this->path.addEdge(startNode.label, destNode.label, edge->capacity, edge->duration);

                currentNode = parentNode;
            } while (currentNode != s);

            //path = parent;
            paths.push_back(bfs_path);
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
            bfsResult = EK_bfs(s, t, parent, residualGraph);
            new_flow = bfsResult.first;
            bfs_path = bfsResult.second;

            if (new_flow == 0) 
                break;            

            // we found a new valid augment path, update path graph

            int currentNode = nodes.size();
            do {
                int parentNode = parent.at(currentNode);

                auto startNode = this->network.getNode(parentNode);
                auto destNode = this->network.getNode(currentNode);

                auto startNodeEdges = startNode.adj;

                auto edge = std::find_if(startNodeEdges.begin(), startNodeEdges.end(), [&destNode](const Edge& edge) -> bool { return edge.dest == destNode.label; });

                if (edge == startNodeEdges.end()) continue;

                if (!this->path.hasNode(startNode.label))
                    this->path.addNode(startNode.label);

                if (!this->path.hasNode(destNode.label))
                    this->path.addNode(destNode.label);

                this->path.addEdge(startNode.label, destNode.label, edge->capacity, edge->duration);

                currentNode = parentNode;
            } while (currentNode != s);

            //path = parent;
            paths.push_back(bfs_path);
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

    return { flow, paths };
}
