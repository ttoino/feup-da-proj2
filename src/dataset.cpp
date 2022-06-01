#include <algorithm>
#include <filesystem>
#include <fstream>
#include <queue>

#include "../includes/constants.hpp"
#include "../includes/dataset.hpp"
#include "../includes/utils.hpp"

Dataset::Dataset(const unsigned int n) {
    for (int i = 1; i <= n; ++i)
        nodes.insert({i, {i}});
    cap.resize(nodes.size() + 1, std::vector<int>(nodes.size() + 1));
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
    cap.at(src).at(dest) = capacity;
}

int Dataset::bfs(int s, int t, std::vector<int>* parent) {

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

            if (parent->at(dest) == -1 && cap.at(cur).at(dest) > 0) {
                parent->at(dest) = cur;
                int new_flow = std::min(flow, cap.at(cur).at(dest));

                if (dest == t) {
                    return new_flow;
                }
                    

                q.push({dest, new_flow});
            }
        }
    }

  return 0;
}

std::pair<int, std::vector<int>> Dataset::edmondsKarp(int s, int t) {
    
    int flow = 0;
    std::vector<int> parent(nodes.size() + 1);

    while (true) {
        int new_flow = bfs(s, t, &parent);
        
        if (new_flow == 0) 
            break;            

        flow += new_flow;
        int cur = t;

        while (cur != s) {
            int prev = parent.at(cur);	
            cap.at(prev).at(cur) -= new_flow;
            cap.at(cur).at(prev) += new_flow;
            cur = prev;
            
        }
    }

    return {flow, parent};
}
