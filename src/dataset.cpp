#include <algorithm>
#include <filesystem>
#include <fstream>
#include <list>
#include <queue>
#include <random>
#include <set>

#include "../includes/constants.hpp"
#include "../includes/dataset.hpp"
#include "../includes/utils.hpp"

Dataset::Dataset(const Graph &graph) : graph(graph) {}

Dataset Dataset::load(const std::string &path) {
    if (path == "output.csv")
        return {};

    std::ifstream dataset_file{DATASETS_PATH + path};

    if (!dataset_file.is_open())
        return {};

    std::string line;
    std::vector<std::string> tokens;

    std::getline(dataset_file, line);

    if (line.empty())
        return {};

    tokens = split(line, ' ');

    int n = stoul(tokens[0]);
    int t = stoul(tokens[1]);

    Graph result{n};

    for (int i = 0; i < t; ++i) {
        std::getline(dataset_file, line);
        tokens = split(line, ' ');

        int src = stoul(tokens[0]), dest = stoul(tokens[1]),
            capacity = stoul(tokens[2]), duration = stoul(tokens[3]);

        result.addEdge(src, dest, capacity, duration);
    }

    Dataset dataset{result};
    return dataset;
}

Dataset Dataset::generate(const std::string &name,
                          const DatasetGenerationParams &params) {
    std::ofstream out{DATASETS_PATH + name + ".txt"};

    out << params.numberOfNodes << ' ' << params.numberOfEdges << '\n';

    Graph graph{params.numberOfNodes};
    auto &nodesMap = graph.getNodes();

    std::vector<std::pair<int, int>>edges{};

    for (int i = 1; i < params.numberOfNodes; ++i)
        for (int j = i + 1; j <= params.numberOfNodes; ++j)
            edges.push_back({i, j});

    // Setup random generators
    std::random_device rd{};
    std::mt19937 gen{rd()};

    std::uniform_int_distribution
        edgeCapacityDist{params.minEdgeCapacity, params.maxEdgeCapacity},
        edgeDurationDist{params.minEdgeDuration, params.maxEdgeDuration};

    std::shuffle(edges.begin(), edges.end(), gen);

    for (int i = 0; i < params.numberOfEdges && i < edges.size(); i++) {
        auto e = edges.at(i);
        
        int capacity = edgeCapacityDist(gen);
        int duration = edgeDurationDist(gen);

        graph.addEdge(e.first, e.second, capacity, duration);

        out << e.first << ' ' << e.second << ' ' << capacity << ' ' << duration << '\n';
    }

    return {graph};
}

std::vector<std::string> Dataset::getAvailableDatasets() {
    std::vector<std::string> result{};

    for (auto &p : std::filesystem::directory_iterator(DATASETS_PATH))
        if (p.is_regular_file() &&
            p.path().filename().string().find("output") == std::string::npos)
            result.push_back(p.path().filename().string());

    std::sort(result.begin(), result.end());

    return result;
}

std::string Dataset::toDotFile() {
    std::stringstream out{};

    // out << "digraph {\n"
    //        "overlap=scale\n"
    //        "splines=true\n"
    //        "node [shape=circle]\n";

    // std::set<std::pair<int, int>> edges;
    // for (auto p : path.getNodes())
    //     for (auto e : p.second.adj)
    //         out << '"' << p.first << "f\" -> \"" << e.dest
    //             << "f\" [label=" << e.capacity << "]\n";

    // for (auto p : path.getNodes())
    //     for (auto e : p.second.adj) {
    //         edges.emplace(p.first, e.dest);
    //         out << p.first << " -> " << e.dest << " [color=red]\n";
    //     }

    // for (auto p : network.getNodes())
    //     for (auto e : p.second.adj)
    //         if (!edges.contains({p.first, e.dest}))
    //             out << p.first << " -> " << e.dest << '\n';

    // out << "}\n";

    return out.str();
}
