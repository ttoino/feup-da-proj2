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

#define INF (std::numeric_limits<int>::max() / 2)

/**
 * @brief Parameters to customize how Dataset::generate() generates datasets.
 */
struct DatasetGenerationParams {
    // TODO
};

enum EdmondsKarpUsage {
    DEFAULT,
    CUSTOM,
};

/**
 * @brief Holds data from a dataset to be used by different scenarios.
 *
 * @details Also has several static methods to make dataset management easier.
 */
class Dataset {

    Graph network, path;

    /**
     * @brief This graph's residual graph in a matrix
     *
     */
    std::vector<std::vector<int>> residualGraph;

    Dataset(const Graph &graph);
    Dataset(){};

    /**
     * @brief Applies breadth-first-search to the graph when running the
     *        Edmonds-Karp algorithm
     *
     * @param s Starting node
     * @param t Destination node
     * @param parent Array to keep the path
     * @param residualGraph The residual graph of a dataset
     *
     * @return The max flow that is available in the path from s to t
     */
    std::pair<int, std::list<int>>
    edmondsKarpBFS(int s, int t, std::vector<int> &parent,
                   std::vector<std::vector<int>> &residualGraph, bool isWholeGragh);

public:
    Graph &getGraph() { return network; }
    Graph &getPath() { return path; }

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
     * @brief Implementation of the Edmonds-Karp algorithm
     *
     * @param s Starting node
     * @param t Destination node
     *
     * @return The max flow from s to t
     */
    std::pair<int, std::vector<std::list<int>>>
    edmondsKarp(int s, int t, EdmondsKarpUsage usage, int groupSize, bool isWholeGragh);

    /**
     * @brief Get the residual graph
     *
     * @return The matrix that represents the residual graph
     */
    std::vector<std::vector<int>> getResidualGraph() { return residualGraph; };
};

#endif
