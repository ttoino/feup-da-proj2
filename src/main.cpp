#include <filesystem>
#include <iostream>
#include <set>

#include "../includes/constants.hpp"
#include "../includes/dataset.hpp"
#include "../includes/ui.hpp"
#include "../includes/utils.hpp"

int main(int argc, char **argv) {
    UserInterface ui{};

    Dataset dataset = Dataset::load("in01_b.txt");

    try {
        while (true)
            ui.show(dataset);
    } catch (Exit) {
        ui.exit();
    }

    return 0;
}
