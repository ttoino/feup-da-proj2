#include <algorithm>
#include <functional>
#include <fstream>
#include <limits>
#include <list>
#include <set>
#include <sstream>
#include <iostream>

#include "../includes/constants.hpp"
#include "../includes/scenarios.hpp"

ScenarioResult::ScenarioResult(int flow, int maxCapacity)
    : flow(flow), maxCapacity(maxCapacity) {}

std::string ScenarioResult::toCSV() const {
    std::stringstream out{};

    out << runtime;

    return out.str();
}

const ScenarioResult scenario1(Dataset &dataset,
                               Scenario1Strategy strat) {

    auto& nodes = dataset.getNodes();

    std::vector<int> capacities(static_cast<int>(nodes.size()) + 1, 0);
    std::set<std::pair<int /* capacity */, int /* node */>, std::greater<std::pair<int, int>>> capacitiesHeap;

    for (auto& [index, node] : nodes) {
        capacitiesHeap.insert({0, index});
        capacities.at(index) = 0;
        node.parent = -1;
    }

    capacitiesHeap.erase({0, 1});
    capacitiesHeap.insert({INF, 1});
    capacities.at(1) = INF;
    nodes[1].parent = 1;

    while (!capacitiesHeap.empty()) {

        int v = capacitiesHeap.extract(capacitiesHeap.begin()).value().second;

        auto& orig = nodes.at(v);

        for (auto& edge : orig.adj) {
            auto& w = edge.dest;

            if (std::min(capacities[v], edge.capacity) > capacities[w]) {

                capacitiesHeap.erase({capacities[w], w});
                capacities[w] = std::min(capacities[v], edge.capacity);
                nodes.at(w).parent = v;
                capacitiesHeap.insert({capacities[w], w});
            }
        }
    }

    return {-1, capacities[50]};
}

const ScenarioResult scenario2(Dataset &dataset,
                               Scenario2Strategy strat) {
    

    std::vector<int> path;

    auto& nodes = dataset.getNodes();
    
    Node startingNode = nodes.at(1);
    Node destinationNode = nodes.at(nodes.size());

    int maxFlow = dataset.edmondsKarp(startingNode.label, destinationNode.label);

    return {maxFlow, -1};
}

void runAllScenarios() {
    std::ofstream out{DATASETS_PATH + OUTPUT_FILE};
    out << OUTPUT_HEADER;

    for (const auto &name : Dataset::getAvailableDatasets()) {
        Dataset dataset = Dataset::load(name);

        FOR_ENUM(Scenario1Strategy, strat) {
            auto result = scenario1(dataset, strat);

            out << name << ',' << 1 << ',' << (int)strat << ','
                << result.toCSV() << '\n';
        }

        FOR_ENUM(Scenario2Strategy, strat) {
            auto result = scenario2(dataset, strat);

            out << name << ',' << 2 << ',' << (int)strat << ','
                << result.toCSV() << '\n';
        }
    }
}
