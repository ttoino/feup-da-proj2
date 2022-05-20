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

    /**
     * @brief Creates a scenario result from the given params.
     *
     * @param remainingOrders The orders that were not delivered.
     * @param vans The vans that were used.
     * @param runtime How much time the algorithm took to run.
     */
    ScenarioResult(const std::chrono::microseconds &runtime);

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
ENUM(Scenario1Strategy, DEFAULT);

const ScenarioResult scenario1(const Dataset &dataset, Scenario1Strategy strat);

/**
 * @brief Holds the possible strategies for scenario2().
 */
ENUM(Scenario2Strategy, DEFAULT);

const ScenarioResult scenario2(const Dataset &dataset, Scenario2Strategy strat);

/**
 * @brief Runs every scenario for every dataset with all possible strategies and
 *        outputs the results to ::OUTPUT_FILE.
 */
void runAllScenarios();

#endif
