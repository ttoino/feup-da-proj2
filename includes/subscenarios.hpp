#ifndef SUBSCENARIOS_HPP
#define SUBSCENARIOS_HPP

#include <algorithm>
#include <functional>
#include <fstream>
#include <limits>
#include <list>
#include <set>
#include <sstream>
#include <iostream>

#include "scenarios.hpp"
#include "dataset.hpp"

/**
 * @brief Maximizes the group dimension and picks any path
 * 
 * @param dataset The graph in which the algorithm is performed
 * @return ScenarioResult The maximum number of elements of the group and a path available for them
 */
ScenarioResult scenario1_1(Dataset &dataset);

/**
 * @brief Calculates 2 possible solutions
 * @details One that minimizes the number of stops, but the group is smaller
 * @details Other that maximizes the number of elements of the group, but there are more stops
 * 
 * @param dataset The graph in which the algorithm is performed
 * @return ScenarioResult The 2 solutions refered above
 */
ScenarioResult scenario1_2(Dataset &dataset);

/**
 * @brief Calculates a path for a given group to use, given the group's size
 * 
 * @param dataset The graph in which the algorithm is performed
 * @return ScenarioResult The path for the group 
 */
ScenarioResult scenario2_1(Dataset &dataset);

/**
 * @brief Calculates a new path for a group with an increased given size 
 * 
 * @param dataset The graph in which the algorithm is performed
 * @return ScenarioResult The new path for the group 
 */
ScenarioResult scenario2_2(Dataset &dataset);

/**
 * @brief Calculates the maximum size of a group and calculates the path 
 * 
 * @param dataset The graph in which the algorithm is performed
 * @return ScenarioResult The maximum number of elements of the group and a path available for them
 */
ScenarioResult scenario2_3(Dataset &dataset);

/**
 * @brief Calculates at what time after departure the group will meet in the destination
 * 
 * @param dataset The graph in which the algorithm is performed
 * @return ScenarioResult The minimum time at which the group meets in the destination
 */
ScenarioResult scenario2_4(Dataset &dataset);

/**
 * @brief Calculates the maximum time some elements of the group wait for the others
 * 
 * @param dataset The graph in which the algorithm is performed
 * @return ScenarioResult The maximum time some elements of the group wait and their locations
 */
ScenarioResult scenario2_5(Dataset &dataset);


#endif
