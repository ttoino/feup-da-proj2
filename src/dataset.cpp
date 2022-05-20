#include <algorithm>
#include <filesystem>

#include "../includes/constants.hpp"
#include "../includes/dataset.hpp"

Dataset::Dataset() {}

Dataset Dataset::load(const std::string &path) {
    // TODO

    return {};
}

Dataset Dataset::generate(const std::string &name,
                          const DatasetGenerationParams &params) {
    // TODO
    std::filesystem::create_directory({DATASETS_PATH + name});

    return {};
}

std::vector<std::string> Dataset::getAvailableDatasets() {
    std::vector<std::string> result{};

    for (auto &p : std::filesystem::directory_iterator(DATASETS_PATH))
        if (p.is_regular_file())
            result.push_back(p.path().filename().string());

    std::sort(result.begin(), result.end());

    return result;
}
