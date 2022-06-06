#include <cmath>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

#include "../includes/constants.hpp"
#include "../includes/dataset.hpp"
#include "../includes/scenarios.hpp"
#include "../includes/ui.hpp"
#include "../includes/utils.hpp"

template <class T>
std::optional<T> UserInterface::optionsMenu(const Options<T> &options) {
    for (int i{1}; i < options.size(); ++i) {
        std::cout << "(" << i << ") " << options.at(i).first << std::endl;
    }

    std::cout << "(0) " << options.front().first << "\n\n";

    unsigned option = getUnsignedInput("Please insert option: ");

    if (option < options.size())
        return options.at(option).second;

    errorMessage = "Invalid option!\n";
    return {};
}

unsigned long UserInterface::getUnsignedInput(const std::string &prompt,
                                              unsigned long min,
                                              unsigned long max) {
    std::string input;
    unsigned long number;
    bool done = false;

    do {
        input = getStringInput(prompt);

        try {
            number = stoul(input);
            done = true;
        } catch (std::invalid_argument) {
            errorMessage = "Invalid input!\n";
            done = false;
        }
    } while (!done || !inRange(number, min, max));

    return number;
}

double UserInterface::getDoubleInput(const std::string &prompt, double min,
                                     double max) {
    std::string input;
    double number;
    bool done = false;

    do {
        input = getStringInput(prompt);

        try {
            number = stod(input);
            done = true;
        } catch (std::invalid_argument) {
            errorMessage = "Invalid input!\n";
            done = false;
        }
    } while (!done || !inRange(number, min, max));

    return number;
}

std::string UserInterface::getStringInput(const std::string &prompt) {
    std::cout << RED_TEXT << errorMessage << RESET_FORMATTING << prompt;
    errorMessage = "";

    std::string input{};

    std::getline(std::cin, input);
    normalizeInput(input);

    if (std::cin.eof())
        throw Exit();

    return input;
}

bool UserInterface::inRange(unsigned long n, unsigned long min,
                            unsigned long max) {
    bool b = (n <= max) && (n >= min);

    if (!b)
        errorMessage = "Value outside allowed range!\n";

    return b;
}

bool UserInterface::inRange(double n, double min, double max) {
    bool b = (n <= max) && (n >= min);

    if (!b)
        errorMessage = "Value outside allowed range!\n";

    return b;
}

void UserInterface::show(Dataset &dataset) {
    std::cout << CLEAR_SCREEN << PROGRAM_NAME << '\n' << std::endl;

    switch (currentMenu) {
    case Menu::MAIN:
        mainMenu();
        break;
    case Menu::CHOOSE_SCENARIO:
        chooseScenarioMenu();
        break;
    case Menu::CHOOSE_DATASET:
        chooseDatasetMenu(dataset);
        break;
    case Menu::GENERATE_DATASET:
        generateDatasetMenu(dataset);
        break;

    case Menu::RENDER_VISUALIZATIONS:
        renderVisualizationsMenu(dataset);
        break;
    case Menu::VISUALIZE_DATASET:
        visualizeDatasetMenu(dataset);
        break;

    case Menu::SCENARIO_ONE:
        scenarioOneMenu(dataset);
        break;

    case Menu::SCENARIO_1_1:
        scenario1_1Menu(dataset);
        break;
    case Menu::SCENARIO_1_2:
        scenario1_2Menu(dataset);
        break;

    case Menu::SCENARIO_TWO:
        scenarioTwoMenu(dataset);
        break;

    case Menu::SCENARIO_2_1:
        scenario2_1Menu(dataset);
        break;
    case Menu::SCENARIO_2_2:
        scenario2_2Menu(dataset);
        break;
    case Menu::SCENARIO_2_3:
        scenario2_3Menu(dataset);
        break;
    case Menu::SCENARIO_2_4:
        scenario2_4Menu(dataset);
        break;
    case Menu::SCENARIO_2_5:
        scenario2_5Menu(dataset);
        break;

    case Menu::ALL_SCENARIOS:
        allScenariosMenu();
        break;

    case Menu::EXIT:
    default:
        throw Exit();
        break;
    }
}

void UserInterface::exit() {
    std::cout << CLEAR_SCREEN << std::flush;
    std::cout << "Shutting down..." << std::endl;
}

void UserInterface::mainMenu() {
    auto menu = optionsMenu<Menu>({
        {"Exit", Menu::EXIT},
        {"Choose dataset", Menu::CHOOSE_DATASET},
        {"Choose scenario", Menu::CHOOSE_SCENARIO},
        {"Generate dataset", Menu::GENERATE_DATASET},
        {"Visualize dataset (requires graphviz)", Menu::RENDER_VISUALIZATIONS},
    });
    currentMenu = menu.value_or(currentMenu);
}

template <class T>
void UserInterface::paginatedMenu(const std::vector<T> &items) {
    static unsigned int page{0};

    unsigned int pages = ceil((float)items.size() / ITEMS_PER_PAGE);

    if (pages == 0) {
        currentMenu = Menu::MAIN;
        getStringInput("\nNo items available, press enter to go back ");
        return;
    }

    for (auto i{items.begin() + page * ITEMS_PER_PAGE},
         end{page == pages - 1 ? items.end()
                               : items.begin() + (page + 1) * ITEMS_PER_PAGE};
         i < end; ++i)
        std::cout << *i << std::endl;

    std::cout << "\nPage " << page + 1 << " of " << pages;
    auto option =
        getStringInput("\nPress enter for next page, or 'q' to exit ");

    if ((option == "q" || option == "Q") || (++page == pages)) {
        currentMenu = Menu::MAIN;
        page = 0;
    }
}

void UserInterface::chooseScenarioMenu() {
    auto menu = optionsMenu<Menu>({
        {"Go back", Menu::MAIN},
        {"Scenario 1 - No group separation", Menu::SCENARIO_ONE},
        {"Scenario 2 - Group separation", Menu::SCENARIO_TWO},
        {"Run all scenarios (with every dataset)", Menu::ALL_SCENARIOS},
    });
    currentMenu = menu.value_or(currentMenu);
}

void UserInterface::chooseDatasetMenu(Dataset &dataset) {
    Options<std::string> options{{"Go back", ""}};

    auto datasets = Dataset::getAvailableDatasets();
    std::transform(
        datasets.begin(), datasets.end(), std::back_inserter(options),
        [](const std::string &n) -> Options<std::string>::value_type {
            return {n, n};
        });

    const auto selection = optionsMenu(options);

    if (!selection.has_value())
        return;

    currentMenu = Menu::MAIN;

    if (selection == "")
        return;

    dataset = Dataset::load(selection.value());
}

void UserInterface::generateDatasetMenu(Dataset &dataset) {
    DatasetGenerationParams params;
    std::string name;
    while (true) {
        name = getStringInput("Dataset name: ");

        if (name == "")
            errorMessage = "Empty name not allowed!\n";
        else if (name.find('/') != std::string::npos)
            errorMessage = "Slashes are not allowed!\n";
        else
            break;
    }

    params.numberOfNodes = getUnsignedInput("Number of nodes to generate: ", 1);
    params.numberOfEdges =
        getUnsignedInput("Number of edges to generate: ", 1,
                         params.numberOfNodes * (params.numberOfNodes - 1) / 2);

    params.minEdgeCapacity = getUnsignedInput("Minimum edge capacity: ", 1);
    params.maxEdgeCapacity =
        getUnsignedInput("Maximum edge capacity: ", params.minEdgeCapacity);

    params.minEdgeDuration = getUnsignedInput("Minimum edge duration: ", 1);
    params.maxEdgeDuration =
        getUnsignedInput("Maximum edge duration: ", params.minEdgeDuration);

    currentMenu = Menu::MAIN;
    dataset = Dataset::generate(name, params);
}

void UserInterface::scenarioOneMenu(Dataset &dataset) {
    auto menu = optionsMenu<Menu>({
        {"Go back", Menu::CHOOSE_SCENARIO},
        {"Maximize group size", Menu::SCENARIO_1_1},
        {"Minimize connections", Menu::SCENARIO_1_2},
    });
    currentMenu = menu.value_or(currentMenu);
}

void UserInterface::scenario1_1Menu(Dataset &dataset) {
    scenario1_1(dataset);

    auto &result = dataset.getScenario1Result();

    std::cout << "Max capacity: " << result.capacity1_1
              << "\nNumber of connections: "
              << result.connections1_1
              << "\nRuntime: "
              << result.runtime1_1.count() << "μs\n\n";

    getStringInput("Press enter to continue ");

    currentMenu = Menu::MAIN;
}

void UserInterface::scenario1_2Menu(Dataset &dataset) {
    scenario1_2(dataset);

    auto &result = dataset.getScenario1Result();

    std::cout << "Max capacity: " << result.capacity1_2
              << "\nNumber of connections: "
              << result.connections1_2
              << "\nRuntime: "
              << result.runtime1_2.count() << "μs\n\n";

    getStringInput("Press enter to continue ");
    
    currentMenu = Menu::MAIN;
}

void UserInterface::scenarioTwoMenu(Dataset &dataset) {
    auto &result = dataset.getScenario2Result();

    Options<Menu> options = {{"Go back", Menu::CHOOSE_SCENARIO}};
    options.emplace_back("Find path from group size", Menu::SCENARIO_2_1);
    if (result.groupSize2_1 != -1)
        options.emplace_back("Increase group size", Menu::SCENARIO_2_2);
    options.emplace_back("Find maximum group size and its path",
                         Menu::SCENARIO_2_3);
    if (result.groupSize2_1 != -1 || result.maxFlow2_3 != -1) {
        options.emplace_back("Find earliest end time", Menu::SCENARIO_2_4);
        options.emplace_back("Find maximum wait time", Menu::SCENARIO_2_5);
    }

    auto menu = optionsMenu<Menu>(options);
    currentMenu = menu.value_or(currentMenu);
}

void UserInterface::scenario2_1Menu(Dataset &dataset) {
    int groupSize = getUnsignedInput("Group size: ");

    scenario2_1(dataset, groupSize);

    auto &result = dataset.getScenario2Result();

    if (result.groupSize2_1 == -1) {
        std::cout << "No path found for that group size!\n";
    }

    std::cout << "Runtime: " << result.runtime2_1.count() << "μs\n\n";

    getStringInput("Press enter to continue ");

    currentMenu = Menu::MAIN;
}

void UserInterface::scenario2_2Menu(Dataset &dataset) {
    auto &result = dataset.getScenario2Result();

    std::stringstream prompt{};
    prompt << "Group size increase (from " << result.groupSize2_1 << "): ";
    int increase = getUnsignedInput(prompt.str());

    scenario2_2(dataset, increase);

    if (result.increase2_2 == -1) {
        std::cout << "No path found for that group size increase!\n";
    } else if (result.requiresNewPath2_2) {
        std::cout << "That increase requires a new path\n";
    } else {
        std::cout << "That increase does not require a new path\n";
    }

    std::cout << "Runtime: " << result.runtime2_2.count() << "μs\n\n";

    getStringInput("Press enter to continue ");

    currentMenu = Menu::MAIN;
}

void UserInterface::scenario2_3Menu(Dataset &dataset) {
    scenario2_3(dataset);

    auto &result = dataset.getScenario2Result();

    std::cout << "Maximum flow: " << result.maxFlow2_3
              << "\nRuntime: " << result.runtime2_3.count() << "μs\n\n";

    getStringInput("Press enter to continue ");

    currentMenu = Menu::MAIN;
}

void UserInterface::scenario2_4Menu(Dataset &dataset) {
    auto &result = dataset.getScenario2Result();

    Options<std::optional<Graph>> options{{"Go back", {}}};

    if (result.groupSize2_1 != -1)
        options.emplace_back("Use path from 2.1", result.path2_1);
    if (result.increase2_2 != -1)
        options.emplace_back("Use path from 2.2", result.path2_2);
    if (result.maxFlow2_3 != -1)
        options.emplace_back("Use path from 2.3", result.path2_3);

    Graph graph{};

    if (options.size() == 2) {
        graph = options.at(1).second.value();
    } else {
        auto selection = optionsMenu(options);

        if (!selection.has_value()) {
            return;
        } else if (!selection.value().has_value()) {
            currentMenu = Menu::MAIN;
            return;
        } else {
            graph = selection.value().value();
        }
    }

    scenario2_4(dataset, graph);

    std::cout << "Earliest end time: " << result.earliestFinish2_4
              << "\nRuntime: " << result.runtime2_4.count() << "μs\n\n";

    getStringInput("Press enter to continue ");

    currentMenu = Menu::MAIN;
}

void UserInterface::scenario2_5Menu(Dataset &dataset) {
    auto &result = dataset.getScenario2Result();

    Options<std::optional<Graph>> options{{"Go back", {}}};

    if (result.groupSize2_1 != -1)
        options.emplace_back("Use path from 2.1", result.path2_1);
    if (result.increase2_2 != -1)
        options.emplace_back("Use path from 2.2", result.path2_2);
    if (result.maxFlow2_3 != -1)
        options.emplace_back("Use path from 2.3", result.path2_3);

    Graph graph{};

    if (options.size() == 2) {
        graph = options.at(1).second.value();
    } else {
        auto selection = optionsMenu(options);

        if (!selection.has_value()) {
            return;
        } else if (!selection.value().has_value()) {
            currentMenu = Menu::MAIN;
            return;
        } else {
            graph = selection.value().value();
        }
    }

    scenario2_5(dataset, graph);

    std::cout << "Maximum wait time: " << result.maxWaitTime2_5
              << "\nRuntime: " << result.runtime2_5.count() << "μs\n\n";

    if (!result.maxWaitNodes2_5.empty()) {
        std::cout << "Maximum wait time at node" << ((result.maxWaitNodes2_5.size() > 1) ? "s" : "") << ": ";

        std::cout << *(result.maxWaitNodes2_5.begin());

        for (auto it = (result.maxWaitNodes2_5.begin() + 1); it < (result.maxWaitNodes2_5.end() - 1); ++it)
            std::cout << ", " << *it;

        if (result.maxWaitNodes2_5.size() > 1)
            std::cout << " and " << *(result.maxWaitNodes2_5.end() - 1);

        std::cout << "\n\n";
    }

    getStringInput("Press enter to continue ");

    currentMenu = Menu::MAIN;
}

void UserInterface::allScenariosMenu() {
    int groupSize = getUnsignedInput("Group size: ");
    int increase = getUnsignedInput("Group size increase: ");

    runAllScenarios(groupSize, increase);

    getStringInput("Press enter to continue ");
    currentMenu = Menu::MAIN;
}

void UserInterface::renderVisualizationsMenu(Dataset &dataset) {
    std::cout << "Rendering visualizations, this may take a while...\n";

    visualizations = dataset.render();

    if (visualizations.empty()) {
        std::cout << "\nCould't render visualizations, make sure graphviz is installed.\n";

        getStringInput("\nPress enter to continue ");
        currentMenu = Menu::MAIN;
    } else {
        currentMenu = Menu::VISUALIZE_DATASET;
    }
}

void UserInterface::visualizeDatasetMenu(Dataset &dataset) {
    Options<std::optional<Visualization>> possibleOptions{
        {"Go back", {}},
        {"Visualize graph", Visualization::DATASET},
        {"Visualize max capacity path", Visualization::SCENARIO_1_1},
        {"Visualize max capacity path only", Visualization::SCENARIO_1_1_ONLY},
        {"Visualize min connections path", Visualization::SCENARIO_1_2},
        {"Visualize min connections path only",
         Visualization::SCENARIO_1_2_ONLY},
        {"Visualize both paths", Visualization::SCENARIO_1},
        {"Visualize path for given group size", Visualization::SCENARIO_2_1},
        {"Visualize path for given group size only",
         Visualization::SCENARIO_2_1_ONLY},
        {"Visualize path for given group size increase",
         Visualization::SCENARIO_2_2},
        {"Visualize path for given group size increase only",
         Visualization::SCENARIO_2_2_ONLY},
        {"Visualize max flow path", Visualization::SCENARIO_2_3},
        {"Visualize max flow path only", Visualization::SCENARIO_2_3_ONLY},
        {"Visualize all three paths", Visualization::SCENARIO_2},
    },
        options{};

    std::copy_if(
        possibleOptions.begin(), possibleOptions.end(),
        std::back_inserter(options),
        [&](const Options<std::optional<Visualization>>::value_type p) {
            return !p.second.has_value() ||
                   visualizations.count(p.second.value());
        });

    auto selection = optionsMenu(options);

    if (!selection.has_value())
        return;

    if (!selection.value().has_value()) {
        currentMenu = Menu::MAIN;
        return;
    }

    auto file = visualizations.at(selection.value().value());

    system(("xdg-open " + file).c_str());
}
