#include "../includes/subscenarios.hpp"

ScenarioResult scenario1_1(Dataset &dataset) {

    auto tstart = std::chrono::high_resolution_clock::now();

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

    int node = nodes.at(nodes.size()).label;
    std::vector<int> path;

    while(node != nodes.at(1).label) {
        path.insert(path.begin(), node);
        node = nodes.at(node).parent;
    }
    path.insert(path.begin(), nodes.at(1).label);

    auto tend = std::chrono::high_resolution_clock::now();

    return {-1, capacities[nodes.size()], -1, path, std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart)};
}

ScenarioResult scenario1_2(Dataset &dataset) {

    auto tstart = std::chrono::high_resolution_clock::now();
    
    auto results = dataset.BFS(1, dataset.getNodes().size());

    auto tend = std::chrono::high_resolution_clock::now();

    return {-1, results.first, -1, results.second, std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart)};
}

ScenarioResult scenario2_1(Dataset &dataset) {
    auto tstart = std::chrono::high_resolution_clock::now();

    int groupSize = 0;
    
    std::vector<int> parents;
    std::vector<int> path;

    auto& nodes = dataset.getNodes();
    EdmondsKarpUsage usage = EdmondsKarpUsage::CUSTOM;
    
    Node startingNode = nodes.at(1);
    Node destinationNode = nodes.at(nodes.size());

    std::cout << "Group size: ";
    std::cin >> groupSize;
    std::cin.ignore(1000, '\n');
    std::cin.clear();

    std::pair<int, std::vector<int>> res = dataset.edmondsKarp(
        startingNode.label, destinationNode.label, usage, groupSize);
    
    int maxFlow = res.first;
    parents = res.second;

    int node = parents.at(nodes.size());
    path.insert(path.begin(), nodes.at(nodes.size()).label);

    while(node != nodes.at(1).label) {
        path.insert(path.begin(), node);
        node = parents.at(node);
    }
    path.insert(path.begin(), nodes.at(1).label);
    
    auto tend = std::chrono::high_resolution_clock::now();

    return {maxFlow, -1, groupSize, path, std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart)};
}

ScenarioResult scenario2_2(Dataset &dataset) {
    //TODO
}

ScenarioResult scenario2_3(Dataset &dataset) {

    auto tstart = std::chrono::high_resolution_clock::now();

    std::vector<int> parents;
    std::vector<int> path;

    auto& nodes = dataset.getNodes();
    
    Node startingNode = nodes.at(1);
    Node destinationNode = nodes.at(nodes.size());

    std::pair<int, std::vector<int>> res = dataset.edmondsKarp(
        startingNode.label, destinationNode.label, EdmondsKarpUsage::DEFAULT, 0);
    
    int maxFlow = res.first;
    parents = res.second;

    int node = parents.at(nodes.size());
    path.insert(path.begin(), nodes.at(nodes.size()).label);

    while(node != nodes.at(1).label) {
        path.insert(path.begin(), node);
        node = parents.at(node);
    }
    path.insert(path.begin(), nodes.at(1).label);

    auto tend = std::chrono::high_resolution_clock::now();

    return {maxFlow, -1, -1, path, std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart)};
}

ScenarioResult scenario2_4(Dataset &dataset) {
    //TODO
}

ScenarioResult scenario2_5(Dataset &dataset) {
    //TOD
}
