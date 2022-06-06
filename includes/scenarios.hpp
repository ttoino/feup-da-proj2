#ifndef DA_PROJ2_SCENARIOS_HPP
#define DA_PROJ2_SCENARIOS_HPP

#include <chrono>
#include <vector>

#include "utils.hpp"
#include "graph.hpp"

/**
 * @brief Data structure holding result values from running simulations related to the first scenario 
 * 
 */
struct Scenario1Result {
    std::chrono::microseconds runtime1_1{0};
    int capacity1_1{-1};
    int connections1_1{-1};
    Graph path1_1{};

    std::chrono::microseconds runtime1_2{0};
    int capacity1_2{-1};
    int connections1_2{-1};
    Graph path1_2{};
};

/**
 * @brief Data structure holding result values from running simulations related to the second scenario 
 * 
 */
struct Scenario2Result {
    std::chrono::microseconds runtime2_1{0};
    int groupSize2_1{-1};
    Graph path2_1{};

    std::chrono::microseconds runtime2_2{0};
    int increase2_2{-1};
    bool requiresNewPath2_2{false};
    Graph path2_2{};

    std::chrono::microseconds runtime2_3{0};
    int maxFlow2_3{-1};
    Graph path2_3{};

    std::chrono::microseconds runtime2_4{0};
    int earliestFinish2_4{-1};

    std::chrono::microseconds runtime2_5{0};
    int maxWaitTime2_5{-1};
    std::vector<int> maxWaitNodes2_5{};
};

/**
 * @brief Runs all scenarios, using all the available datasets.
 * 
 */
void runAllScenarios(int groupSize, int increase);

#include "dataset.hpp"

/**
 * @brief Maximizes the group dimension and picks any path
 *
 * @param dataset The graph in which the algorithm is performed
 * 
 * @return ScenarioResult The maximum number of elements of the group and a path
 * available for them
 */
void scenario1_1(Dataset &dataset);

/**
 * @brief Calculates 2 possible solutions
 * @details One that minimizes the number of stops, but the group is smaller
 * @details Other that maximizes the number of elements of the group, but there
 * are more stops
 *
 * @param dataset The graph in which the algorithm is performed
 * @return ScenarioResult The 2 solutions refered above
 */
void scenario1_2(Dataset &dataset);

/**
 * @brief Calculates a path for a given group to use, given the group's size
 *
 * @param dataset The graph in which the algorithm is performed
 * 
 * @return ScenarioResult The path for the group
 */
void scenario2_1(Dataset &dataset, int groupSize);

/**
 * @brief Calculates a new path for a group with an increased given size
 *
 * @param dataset The graph in which the algorithm is performed
 * 
 * @return ScenarioResult The new path for the group
 */
void scenario2_2(Dataset &dataset, int increase);

/**
 * @brief Calculates the maximum size of a group and calculates the path
 *
 * @param dataset The graph in which the algorithm is performed
 * 
 * @return ScenarioResult The maximum number of elements of the group and a path
 * available for them
 */
void scenario2_3(Dataset &dataset);

/**
 * @brief Calculates at what time after departure the group will meet in the
 * destination
 *
 * @param dataset The graph in which the algorithm is performed
 * 
 * @return ScenarioResult The minimum time at which the group meets in the
 * destination
 */
void scenario2_4(Dataset &dataset, Graph &graph);

/**
 * @brief Calculates the maximum time some elements of the group wait for the
 * others
 *
 * @param dataset The graph in which the algorithm is performed
 * 
 * @return ScenarioResult The maximum time some elements of the group wait and
 * their locations
 */
void scenario2_5(Dataset &dataset, Graph &graph);

#endif
