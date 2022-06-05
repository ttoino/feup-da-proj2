#include <queue>
#include <unordered_map>
#include <map>

#include "../includes/subscenarios.hpp"

ScenarioResult scenario1_1(Dataset &dataset) {
    auto tstart = std::chrono::high_resolution_clock::now();

    auto &nodes = dataset.getGraph().getNodes();

    std::vector<int> capacities(static_cast<int>(nodes.size()) + 1, 0);
    std::set<std::pair<int /* capacity */, int /* node */>,
             std::greater<std::pair<int, int>>>
        capacitiesHeap;
    std::vector<std::list<int>> paths;

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

    int node = nodes.at(nodes.size()).label;
    std::list<int> path;

    while (node != nodes.at(1).label) {
        path.insert(path.begin(), node);
        node = nodes.at(node).parent;
    }
    path.insert(path.begin(), nodes.at(1).label);
    paths.push_back(path);

    auto tend = std::chrono::high_resolution_clock::now();

    return {
        -1,
        capacities[nodes.size()],
        -1,
        paths,
        std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart),
        -1,
        -1,
        {}
    };
}

ScenarioResult scenario1_2(Dataset &dataset) {
    auto tstart = std::chrono::high_resolution_clock::now();

    std::vector<std::list<int>> paths;

    auto results =
        dataset.getGraph().BFS(1, dataset.getGraph().getNodes().size());
    paths.push_back(results.second);

    auto tend = std::chrono::high_resolution_clock::now();

    return {
        -1,
        results.first,
        -1,
        paths,
        std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart),
        -1,
        -1,
        {}
    };
}

ScenarioResult scenario2_1(Dataset &dataset) {
    auto tstart = std::chrono::high_resolution_clock::now();

    int groupSize = 0;

    std::vector<std::list<int>> paths;

    auto &nodes = dataset.getGraph().getNodes();
    EdmondsKarpUsage usage = EdmondsKarpUsage::CUSTOM;

    Node startingNode = nodes.at(1);
    Node destinationNode = nodes.at(nodes.size());

    std::cout << "Group size: ";
    std::cin >> groupSize;
    std::cin.ignore(1000, '\n');
    std::cin.clear();

    std::pair<int, std::vector<std::list<int>>> res = dataset.edmondsKarp(
        startingNode.label, destinationNode.label, usage, groupSize, true);

    int maxFlow = res.first;

    paths = (maxFlow < groupSize) ? paths : res.second;

    auto tend = std::chrono::high_resolution_clock::now();

    return {
        maxFlow,
        -1,
        groupSize,
        paths,
        std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart),
        -1,
        -1,
        {}
    };
}

ScenarioResult scenario2_2(Dataset &dataset) {
    auto tstart = std::chrono::high_resolution_clock::now();
    
    int groupSizeIncrement = 0;
    int initialGroupSize = scenario2_1(dataset).groupSize;

    std::vector<std::list<int>> paths;

    auto &nodes = dataset.getPath().getNodes();
    EdmondsKarpUsage usage = EdmondsKarpUsage::CUSTOM;

    Node startingNode = nodes.at(1);
    Node destinationNode = nodes.at(dataset.getGraph().getNodes().size());

    std::cout << "Group size increment: ";
    std::cin >> groupSizeIncrement;
    std::cin.ignore(1000, '\n');
    std::cin.clear();

    int newGroupSize = initialGroupSize + groupSizeIncrement;

    std::pair<int, std::vector<std::list<int>>> res = dataset.edmondsKarp(
        startingNode.label, destinationNode.label, usage, newGroupSize, false);
    
    if(res.first < newGroupSize) {
        std::cout << "The group can't travel through the current path\n";
        std::pair<int, std::vector<std::list<int>>> res = dataset.edmondsKarp(
            startingNode.label, destinationNode.label, usage, newGroupSize, true);
    }

    int maxFlow = res.first;
    
    paths = (maxFlow < newGroupSize) ? paths : res.second;

    auto tend = std::chrono::high_resolution_clock::now();

    return {
        maxFlow,
        -1,
        newGroupSize,
        paths,
        std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart),
        -1,
        -1,
        {}
    };
}

ScenarioResult scenario2_3(Dataset &dataset) {

    auto tstart = std::chrono::high_resolution_clock::now();

    std::vector<std::list<int>> paths;

    auto &nodes = dataset.getGraph().getNodes();

    Node startingNode = nodes.at(1);
    Node destinationNode = nodes.at(nodes.size());

    std::pair<int, std::vector<std::list<int>>> res =
        dataset.edmondsKarp(startingNode.label, destinationNode.label,
                            EdmondsKarpUsage::DEFAULT, 0, true);

    int maxFlow = res.first;
    paths = res.second;

    auto tend = std::chrono::high_resolution_clock::now();

    return {
        maxFlow,
        -1,
        -1,
        paths,
        std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart),
        -1,
        -1,
        {}
    };
}

ScenarioResult scenario2_4(Dataset &dataset) {

    scenario2_1(dataset);

    auto& nodes = dataset.getPath().getNodes();

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

    return {
        -1,
        -1,
        -1,
        std::vector<std::list<int>>(),
        static_cast<std::chrono::microseconds>(0),
        minDuration,
        -1,
        {}
    };
}

ScenarioResult scenario2_5(Dataset &dataset) {

    scenario2_4(dataset);

    std::multimap<int, int> waitTimes; // ordering is useful to retrieve the maxWaitTime

    auto& nodes = dataset.getPath().getNodes();

    for (const auto& [index, node] : nodes)
        waitTimes.insert({node.maxTime - node.minTime, index});

    auto maxWaitTime = waitTimes.rbegin()->first;

    std::vector<int> maxWaitNodes;
    auto maxWaitTimeRange = waitTimes.equal_range(maxWaitTime);
    for (auto it = maxWaitTimeRange.first; it != maxWaitTimeRange.second; ++it)
        maxWaitNodes.push_back(it->second);

    return {
        -1,
        -1,
        -1,
        std::vector<std::list<int>>(),
        static_cast<std::chrono::microseconds>(0),
        -1,
        maxWaitTime,
        maxWaitNodes
    };
}
