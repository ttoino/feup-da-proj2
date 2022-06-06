#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <queue>
#include <set>
#include <sstream>
#include <map>

#include "../includes/constants.hpp"
#include "../includes/scenarios.hpp"

void runAllScenarios() {
    std::ofstream out{OUTPUT_PATH + OUTPUT_FILE};
    out << OUTPUT_HEADER;

    for (const auto &name : Dataset::getAvailableDatasets()) {
        Dataset dataset = Dataset::load(name);

        // FOR_ENUM(Scenario1Strategy, strat) {
        //     auto result = scenario1(dataset, strat);

        //     out << name << ',' << 1 << ',' << (int)strat << ','
        //         << result.toCSV() << '\n';
        // }

        // FOR_ENUM(Scenario2Strategy, strat) {
        //     auto result = scenario2(dataset, strat);

        //     out << name << ',' << 2 << ',' << (int)strat << ','
        //         << result.toCSV() << '\n';
        // }
    }
}

void scenario1_1(Dataset &dataset) {
    auto tstart = std::chrono::high_resolution_clock::now();

    auto &nodes = dataset.getGraph().getNodes();

    std::vector<int> capacities(static_cast<int>(nodes.size()) + 1, 0);
    std::set<std::pair<int /* capacity */, int /* node */>,
             std::greater<std::pair<int, int>>>
        capacitiesHeap;

    for (auto &[index, node] : nodes) {
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

        auto &orig = nodes.at(v);

        for (auto &edge : orig.adj) {
            auto &w = edge.dest;

            if (std::min(capacities[v], edge.capacity) > capacities[w]) {
                capacitiesHeap.erase({capacities[w], w});
                capacities[w] = std::min(capacities[v], edge.capacity);
                nodes.at(w).parent = v;
                capacitiesHeap.insert({capacities[w], w});
            }
        }
    }

    auto &result = dataset.getScenario1Result();
    result.path1_1.clear();
    int node = nodes.size();
    result.path1_1.addNode(node);
    result.connections1_1 = 0;

    while (node != 1) {
        result.path1_1.addNode(nodes.at(node).parent);
        result.path1_1.addEdge(nodes.at(node).parent, node, -1, -1);
        ++result.connections1_1;
        node = nodes.at(node).parent;
    }

    auto tend = std::chrono::high_resolution_clock::now();

    result.capacity1_1 = capacities[nodes.size()];
    result.runtime1_1 =
        std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart);
}

void scenario1_2(Dataset &dataset) {
    auto tstart = std::chrono::high_resolution_clock::now();

    auto &nodes = dataset.getGraph().getNodes();
    dataset.getGraph().bfs(1, nodes.size());

    auto &result = dataset.getScenario1Result();
    result.path1_2.clear();
    int node = nodes.size();
    result.path1_2.addNode(node);
    result.connections1_2 = 0;
    result.capacity1_2 = INT_MAX;

    while (node != 1) {
        auto edges = nodes.at(nodes.at(node).parent).adj;
        auto edge = *std::find_if(edges.begin(), edges.end(),
                                  [&](const Edge e) { return e.dest == node; });
        if (edge.capacity < result.capacity1_2)
            result.capacity1_2 = edge.capacity;

        result.path1_2.addNode(nodes.at(node).parent);
        result.path1_2.addEdge(nodes.at(node).parent, node, -1, -1);
        ++result.connections1_2;
        node = nodes.at(node).parent;
    }

    auto tend = std::chrono::high_resolution_clock::now();

    result.runtime1_2 =
        std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart);
}

void scenario2_1(Dataset &dataset, int groupSize) {
    auto tstart = std::chrono::high_resolution_clock::now();

    auto [flow, graph] = dataset.getGraph().edmondsKarp(
        1, dataset.getGraph().getNodes().size(), groupSize);

    auto &result = dataset.getScenario2Result();
    result.path2_1 = graph;
    result.groupSize2_1 = groupSize;

    auto tend = std::chrono::high_resolution_clock::now();

    result.runtime2_1 =
        std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart);
}

void scenario2_2(Dataset &dataset, int increase) {
    auto tstart = std::chrono::high_resolution_clock::now();

    auto &result = dataset.getScenario2Result();
    result.increase2_2 = increase;
    int newGroupSize = result.groupSize2_1 + increase;

    auto [flow, graph] =
        result.path2_1.edmondsKarp(1, dataset.getGraph().getNodes().size());

    result.requiresNewPath2_2 = flow < newGroupSize;
    result.path2_2 = graph;

    if (result.requiresNewPath2_2) {
        auto [flow, graph] = dataset.getGraph().edmondsKarp(
            1, dataset.getGraph().getNodes().size());
        result.path2_2 = graph;
    }

    auto tend = std::chrono::high_resolution_clock::now();

    result.runtime2_2 =
        std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart);
}

void scenario2_3(Dataset &dataset) {
    auto tstart = std::chrono::high_resolution_clock::now();

    auto [flow, graph] =
        dataset.getGraph().edmondsKarp(1, dataset.getGraph().getNodes().size());

    auto &result = dataset.getScenario2Result();
    result.path2_3 = graph;
    result.maxFlow2_3 = flow;

    auto tend = std::chrono::high_resolution_clock::now();

    result.runtime2_3 =
        std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart);
}

void scenario2_4(Dataset &dataset, Graph &graph) {
    auto tstart = std::chrono::high_resolution_clock::now();

    auto& nodes = graph.getNodes();

    std::unordered_map<int, int> earliestStart;
    std::unordered_map<int, int> entryDegree;
    int minDuration = -1;

    for (const auto &[index, node] : nodes) {
        // just to guarantee that nodes with degree 0 are added too
        entryDegree[index] += 0;
        earliestStart[index] = 0;

        for (const auto &edge : node.adj) {
            int dest = edge.dest;

            entryDegree[dest]++;
        }
    }

    std::queue<int> s;

    for (const auto &[index, degree] : entryDegree) {
        if (degree == 0)
            s.push(index);
    }

    while (!s.empty()) {
        int v = s.front();
        s.pop();

        if (minDuration < earliestStart[v])
            minDuration = earliestStart[v];

        auto node = nodes.find(v);

        for (const auto &edge : node->second.adj) {
            int w = edge.dest;

            if (earliestStart[w] < earliestStart[v] + edge.duration)
                earliestStart[w] = earliestStart[v] + edge.duration;
            
            auto& destNode = nodes[w];
            destNode.maxTime = std::max(destNode.maxTime, earliestStart[w]);
            destNode.minTime = std::min(destNode.minTime, earliestStart[w]);

            if (--entryDegree[w] == 0)
                s.push(w);
        }
    }

    auto &result = dataset.getScenario2Result();
    result.earliestFinish2_4 = minDuration;

    auto tend = std::chrono::high_resolution_clock::now();

    result.runtime2_4 =
        std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart);
}

void scenario2_5(Dataset &dataset, Graph &graph) {
    auto tstart = std::chrono::high_resolution_clock::now();

    scenario2_4(dataset, graph);

    // ordering is useful to retrieve the maxWaitTime
    std::multimap<int, int> waitTimes;

    auto &nodes = graph.getNodes();
    auto &result = dataset.getScenario2Result();

    for (const auto &[index, node] : nodes)
        waitTimes.insert({index != 1 ? node.maxTime - node.minTime : 0, index});

    result.maxWaitTime2_5 = waitTimes.rbegin()->first;

    result.maxWaitNodes2_5.clear();
    auto maxWaitTimeRange = waitTimes.equal_range(result.maxWaitTime2_5);
    for (auto it = maxWaitTimeRange.first; it != maxWaitTimeRange.second; ++it)
        result.maxWaitNodes2_5.push_back(it->second);

    auto tend = std::chrono::high_resolution_clock::now();

    result.runtime2_5 =
        std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart);
}
