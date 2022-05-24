#ifndef DA_PROJ2_DATASET_H
#define DA_PROJ2_DATASET_H

struct DatasetGenerationParams;
class Dataset;

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

/**
 * @brief Parameters to customize how Dataset::generate() generates datasets.
 */
struct DatasetGenerationParams {
    // TODO
};

/**
 * @brief Represents an edge connecting two nodes.
 */
struct Edge {
    /**
     * @brief The destination node.
     */
    int dest;
    int capacity;
    int duration;

    Edge(const int dest, const int capacity, const int duration)
        : dest(dest), capacity(capacity), duration(duration){};
    Edge(){};

    bool operator==(const Edge &other) const { return dest == other.dest; }
};

namespace std {
    template <> struct hash<Edge> {
        std::size_t operator()(const Edge &e) const { return e.dest; }
    };
}

/**
 * @brief Represents a node in a Graph.
 */
struct Node {
    /**
     * @brief The outgoing edges connecting this node to others.
     */
    std::unordered_set<Edge> adj{};

    int label = -1;

    Node(const int label) : label(label){};
    Node(){};
};

/**
 * @brief Holds data from a dataset to be used by different scenarios.
 *
 * @details Also has several static methods to make dataset management easier.
 */
class Dataset {
    /**
     * @brief This graph's nodes.
     *
     * The keys are each node's stop code and the values are the nodes
     * themselves.
     */
    std::unordered_map<int, Node> nodes;

    // TODO

    Dataset(const unsigned int n);
    Dataset();

public:
    void addEdge(const int src, const int &dest, const int capacity,
                 const int duration);
    void addEdge(Node &src, Node &dest, const int capacity, const int duration);

    /**
     * @return This graph's nodes.
     */
    std::unordered_map<int, Node> &getNodes() { return nodes; };
    /**
     * @brief Get the node with the specified code.
     *
     * @param id The stop code.
     * @return The node.
     */
    Node &getNode(const int &id) { return nodes.at(id); };

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
};

#endif
