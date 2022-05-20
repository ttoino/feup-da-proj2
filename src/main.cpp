#include <filesystem>
#include <iostream>
#include <set>

#include "../includes/constants.hpp"
#include "../includes/dataset.hpp"
#include "../includes/ui.hpp"
#include "../includes/utils.hpp"

std::string DATASETS_PATH;

int main(int argc, char **argv) {
    // Taken from https://stackoverflow.com/a/55579815/9937109
    DATASETS_PATH = std::filesystem::weakly_canonical(argv[0])
                        .parent_path()
                        .append("datasets/")
                        .string();

    UserInterface ui{};

    Dataset dataset = Dataset::load("default");

    try {
        while (true)
            ui.show(dataset);
    } catch (Exit) {
        ui.exit();
    }

    return 0;
}
