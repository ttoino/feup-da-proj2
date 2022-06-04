#ifndef DA_PROJ2_SCENARIOS_HPP
#define DA_PROJ2_SCENARIOS_HPP

#include <chrono>
#include <functional>
#include <utility>
#include <vector>

#include "dataset.hpp"
#include "utils.hpp"

/**
 * @brief Represents the results of a scenario.
 */
struct ScenarioResult {
    // TODO

    /** @brief How much time the algorithm took to run. */
    std::chrono::microseconds runtime{0};
    std::vector<std::list<int>> paths = {};
    int flow = -1, maxCapacity = -1, groupSize = -1, minDuration = -1;

    /**
     * @brief Creates a scenario result from the given params.
     *
     * @param remainingOrders The orders that were not delivered.
     * @param vans The vans that were used.
     * @param runtime How much time the algorithm took to run.
     */
    ScenarioResult(int flow, int maxCapacity, int groupSize,
                   std::vector<std::list<int>> paths,
                   const std::chrono::microseconds &runtime, int minDuration);

    ScenarioResult(){};

    /**
     * @brief Converts this object to a csv representation.
     *
     * @return A csv representation of this object.
     */
    std::string toCSV() const;
};

/**
 * @brief Holds the possible strategies for scenario1().
 */
ENUM(Scenario1Strategy, FIRST, SECOND);

const ScenarioResult scenario1(Dataset &dataset, Scenario1Strategy strat);

/**
 * @brief Holds the possible strategies for scenario2().
 */
ENUM(Scenario2Strategy, FIRST, SECOND, THIRD, FOURTH, FIFTH);

const ScenarioResult scenario2(Dataset &dataset, Scenario2Strategy strat);

/**
 * @brief Runs every scenario for every dataset with all possible strategies and
 *        outputs the results to ::OUTPUT_FILE.
 */
void runAllScenarios();

#endif
