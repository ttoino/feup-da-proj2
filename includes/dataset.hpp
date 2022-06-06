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
    /** @brief The number of nodes to generate. */
    int numberOfNodes;
    /** @brief The number of edges to generate. */
    int numberOfEdges;

    /** @brief The minimum capacity edges will have. */
    unsigned int minEdgeCapacity;
    /** @brief The maximum capacity edges will have. */
    unsigned int maxEdgeCapacity;

    /** @brief The minimum duration edges will have. */
    unsigned int minEdgeDuration;
    /** @brief The maximum duration edges will have. */
    unsigned int maxEdgeDuration;
};

/** @brief An enum representing a type of visualization to do on a dataset. */
enum class Visualization {
    /** @brief Show the whole graph. */
    DATASET,
    /** @brief Show the paths from the first scenario over the whole graph. */
    SCENARIO_1,
    /** @brief Show the path from scenario 1.1 over the whole graph. */
    SCENARIO_1_1,
    /** @brief Show the path from scenario 1.1. */
    SCENARIO_1_1_ONLY,
    /** @brief Show the path from scenario 1.2 over the whole graph. */
    SCENARIO_1_2,
    /** @brief Show the path from scenario 1.2. */
    SCENARIO_1_2_ONLY,
    /** @brief Show the paths from the second scenario over the whole graph. */
    SCENARIO_2,
    /** @brief Show the path from scenario 2.1 over the whole graph. */
    SCENARIO_2_1,
    /** @brief Show the path from scenario 2.1. */
    SCENARIO_2_1_ONLY,
    /** @brief Show the path from scenario 2.2 over the whole graph. */
    SCENARIO_2_2,
    /** @brief Show the path from scenario 2.2. */
    SCENARIO_2_2_ONLY,
    /** @brief Show the path from scenario 2.3 over the whole graph. */
    SCENARIO_2_3,
    /** @brief Show the path from scenario 2.3. */
    SCENARIO_2_3_ONLY
};

/**
 * @brief Holds data from a dataset to be used by different scenarios.
 *
 * @details Also has several static methods to make dataset management easier.
 */
class Dataset {

    /** @brief How many nodes the graph has. */
    int n = -1;
    /** @brief The graph associated with this dataset. */
    Graph graph;

    /** @brief The results from running the first scenario. */
    Scenario1Result scenario1Result;
    /** @brief The results from running the second scenario. */
    Scenario2Result scenario2Result;

    /** @brief Creates a dataset with n nodes and the given graph. */
    Dataset(const int n, const Graph &graph);
    /** @brief Creates an empty dataset. */
    Dataset(){};

public:
    /**
     * @brief Get the Graph object associated with this dataset
     *
     * @return Graph& the Graph object associated with this dataset
     */
    Graph &getGraph() { return graph; }

    /**
     * @brief Get the number of nodes the graph associated with this dataset has
     *
     * @return int the number of nodes the graph associated with this dataset has
     */
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
