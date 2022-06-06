#ifndef DA_PROJ2_DATASET_H
#define DA_PROJ2_DATASET_H

struct DatasetGenerationParams;
class Dataset;

#include <climits>
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "./graph.hpp"
#include "./scenarios.hpp"

#define INF (std::numeric_limits<int>::max() / 2)

/**
 * @brief Parameters to customize how Dataset::generate() generates datasets.
 */
struct DatasetGenerationParams {
    int numberOfNodes;
    int numberOfEdges;

    unsigned int minEdgeCapacity;
    unsigned int maxEdgeCapacity;

    unsigned int minEdgeDuration;
    unsigned int maxEdgeDuration;
};

enum class Visualization {
    DATASET,
    SCENARIO_1,
    SCENARIO_1_1,
    SCENARIO_1_1_ONLY,
    SCENARIO_1_2,
    SCENARIO_1_2_ONLY,
    SCENARIO_2,
    SCENARIO_2_1,
    SCENARIO_2_1_ONLY,
    SCENARIO_2_2,
    SCENARIO_2_2_ONLY,
    SCENARIO_2_3,
    SCENARIO_2_3_ONLY
};

/**
 * @brief Holds data from a dataset to be used by different scenarios.
 *
 * @details Also has several static methods to make dataset management easier.
 */
class Dataset {

    int n = -1;
    Graph graph;

    Scenario1Result scenario1Result;
    Scenario2Result scenario2Result;

    Dataset(const int n, const Graph &graph);
    Dataset(){};

public:
    /**
     * @brief Get the Graph object associated with this dataset
     *
     * @return Graph& the Graph object associated with this dataset
     */
    Graph &getGraph() { return graph; }

    int getN() { return n; }

    /**
     * @brief Get the results for running the first scenario.
     *
     * @return Scenario1Result& the results for running the first scenario
     */
    Scenario1Result &getScenario1Result() { return scenario1Result; }

    /**
     * @brief Get the results for running the second scenario.
     *
     * @return Scenario1Result& the results for running the second scenario
     */
    Scenario2Result &getScenario2Result() { return scenario2Result; }

    /**
     * @brief Loads a dataset from the given path.
     *
     * @note The path must be relative to ::DATASETS_PATH.
     *
     * @param path The folder where the dataset is.
     *
     * @return The dataset that was loaded.
     */
    static Dataset load(const std::string &path);

    /**
     * @brief Generates a new dataset from pseudo random data and stores it in a
     *        folder.
     *
     * @param name The folder where the dataset will be stored.
     * @param params The parameters given to the random number generators.
     *
     * @return The dataset that was generated.
     */
    static Dataset generate(const std::string &name,
                            const DatasetGenerationParams &params);

    /**
     * @brief Gets all the datasets available to be loaded.
     *
     * @return A vector with the datasets in the ::DATASETS_PATH.
     */
    static std::vector<std::string> getAvailableDatasets();

    /**
     * @brief Creates visualizations ou of this dataset's scenarios
     *
     * @return std::unordered_map<Visualization, std::string>
     */
    std::unordered_map<Visualization, std::string> render();
};

#endif
