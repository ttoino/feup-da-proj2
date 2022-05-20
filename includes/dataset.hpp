#ifndef DA_PROJ2_DATASET_H
#define DA_PROJ2_DATASET_H

struct DatasetGenerationParams;
class Dataset;

#include <string>
#include <vector>

/**
 * @brief Parameters to customize how Dataset::generate() generates datasets.
 */
struct DatasetGenerationParams {
    // TODO
};

/**
 * @brief Holds data from a dataset to be used by different scenarios.
 *
 * @details Also has several static methods to make dataset management easier.
 */
class Dataset {
    // TODO

    Dataset();

public:
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
