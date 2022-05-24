#include <algorithm>
#include <filesystem>
#include <fstream>

#include "../includes/constants.hpp"
#include "../includes/dataset.hpp"
#include "../includes/utils.hpp"

Dataset::Dataset(const unsigned int n) {
    for (int i = 1; i <= n; ++i)
        nodes.insert({i, {i}});
}

Dataset::Dataset() {}

Dataset Dataset::load(const std::string &path) {
    std::ifstream dataset_file{DATASETS_PATH + path};

    if (!dataset_file.is_open())
        return {};

    std::string line;
    std::vector<std::string> tokens;

    std::getline(dataset_file, line);
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
        if (p.is_regular_file())
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
}
