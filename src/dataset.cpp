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

std::unordered_map<Visualization, std::string> Dataset::render() {
    std::unordered_map<Visualization, std::string> m{};

    // DATASET
    graph.toDotFile(OUTPUT_PATH + "dataset.dot");
    std::stringstream command{};
    command << "sfdp -T svg " << OUTPUT_PATH << "dataset.dot > " << OUTPUT_PATH
            << "dataset.svg";
    int r = system(command.str().c_str());
    if (!r)
        m.insert({Visualization::DATASET, OUTPUT_PATH + "dataset.svg"});

    // 1.1
    if (scenario1Result.capacity1_1 != -1) {
        scenario1Result.path1_1.toDotFile(OUTPUT_PATH + "1.1only.dot");
        command = {};
        command << "sfdp -T svg " << OUTPUT_PATH << "1.1only.dot > "
                << OUTPUT_PATH << "1.1only.svg";
        r = system(command.str().c_str());
        if (!r)
            m.insert({Visualization::SCENARIO_1_1_ONLY,
                      OUTPUT_PATH + "1.1only.svg"});

        graph.toDotFile(OUTPUT_PATH + "1.1.dot", {scenario1Result.path1_1});
        command = {};
        command << "sfdp -T svg " << OUTPUT_PATH << "1.1.dot > " << OUTPUT_PATH
                << "1.1.svg";
        r = system(command.str().c_str());
        if (!r)
            m.insert({Visualization::SCENARIO_1_1, OUTPUT_PATH + "1.1.svg"});
    }

    // 1.2
    if (scenario1Result.capacity1_2 != -1) {
        scenario1Result.path1_2.toDotFile(OUTPUT_PATH + "1.2only.dot");
        command = {};
        command << "sfdp -T svg " << OUTPUT_PATH << "1.2only.dot > "
                << OUTPUT_PATH << "1.2only.svg";
        r = system(command.str().c_str());
        if (!r)
            m.insert({Visualization::SCENARIO_1_2_ONLY,
                      OUTPUT_PATH + "1.2only.svg"});

        graph.toDotFile(OUTPUT_PATH + "1.2.dot", {scenario1Result.path1_2});
        command = {};
        command << "sfdp -T svg " << OUTPUT_PATH << "1.2.dot > " << OUTPUT_PATH
                << "1.2.svg";
        r = system(command.str().c_str());
        if (!r)
            m.insert({Visualization::SCENARIO_1_2, OUTPUT_PATH + "1.2.svg"});
    }

    // 1.1 and 1.2
    if (scenario1Result.capacity1_1 != -1 && scenario1Result.capacity1_2 != -1) {
        graph.toDotFile(OUTPUT_PATH + "1.1.dot", {scenario1Result.path1_1, scenario1Result.path1_2});
        command = {};
        command << "sfdp -T svg " << OUTPUT_PATH << "1.1.dot > " << OUTPUT_PATH
                << "1.1.svg";
        r = system(command.str().c_str());
        if (!r)
            m.insert({Visualization::SCENARIO_1, OUTPUT_PATH + "1.1.svg"});
    }

    // 2.1
    if (scenario2Result.groupSize2_1 != -1) {
        scenario2Result.path2_1.toDotFile(OUTPUT_PATH + "2.1only.dot");
        command = {};
        command << "sfdp -T svg " << OUTPUT_PATH << "2.1only.dot > "
                << OUTPUT_PATH << "2.1only.svg";
        r = system(command.str().c_str());
        if (!r)
            m.insert({Visualization::SCENARIO_2_1_ONLY,
                      OUTPUT_PATH + "2.1only.svg"});

        graph.toDotFile(OUTPUT_PATH + "2.1.dot", {scenario2Result.path2_1});
        command = {};
        command << "sfdp -T svg " << OUTPUT_PATH << "2.1.dot > " << OUTPUT_PATH
                << "2.1.svg";
        r = system(command.str().c_str());
        if (!r)
            m.insert({Visualization::SCENARIO_2_1, OUTPUT_PATH + "2.1.svg"});
    }

    // 2.2
    if (scenario2Result.increase2_2 != -1) {
        scenario2Result.path2_2.toDotFile(OUTPUT_PATH + "2.2only.dot");
        command = {};
        command << "sfdp -T svg " << OUTPUT_PATH << "2.2only.dot > "
                << OUTPUT_PATH << "2.2only.svg";
        r = system(command.str().c_str());
        if (!r)
            m.insert({Visualization::SCENARIO_2_2_ONLY,
                      OUTPUT_PATH + "2.2only.svg"});

        graph.toDotFile(OUTPUT_PATH + "2.2.dot", {scenario2Result.path2_2});
        command = {};
        command << "sfdp -T svg " << OUTPUT_PATH << "2.2.dot > " << OUTPUT_PATH
                << "2.2.svg";
        r = system(command.str().c_str());
        if (!r)
            m.insert({Visualization::SCENARIO_2_2, OUTPUT_PATH + "2.2.svg"});
    }

    // 2.3
    if (scenario2Result.maxFlow2_3 != -1) {
        scenario2Result.path2_3.toDotFile(OUTPUT_PATH + "2.3only.dot");
        command = {};
        command << "sfdp -T svg " << OUTPUT_PATH << "2.3only.dot > "
                << OUTPUT_PATH << "2.3only.svg";
        r = system(command.str().c_str());
        if (!r)
            m.insert({Visualization::SCENARIO_2_3_ONLY,
                      OUTPUT_PATH + "2.3only.svg"});

        graph.toDotFile(OUTPUT_PATH + "2.3.dot", {scenario2Result.path2_3});
        command = {};
        command << "sfdp -T svg " << OUTPUT_PATH << "2.3.dot > " << OUTPUT_PATH
                << "2.3.svg";
        r = system(command.str().c_str());
        if (!r)
            m.insert({Visualization::SCENARIO_2_3, OUTPUT_PATH + "2.3.svg"});
    }

    return m;
}
