#include <cmath>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

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

    case Menu::SCENARIO_ONE:
        scenarioOneMenu(dataset);
        break;
    case Menu::SCENARIO_TWO:
        scenarioTwoMenu(dataset);
        break;
    case Menu::ALL_SCENARIOS:
        allScenariosMenu();
        break;

    case Menu::RESULTS:
        resultsMenu();
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

    currentMenu = Menu::MAIN;
    dataset = Dataset::generate(name, params);
}

void UserInterface::scenarioOneMenu(Dataset &dataset) {
    auto selection = optionsMenu<std::optional<Scenario1Strategy>>({
        {"Go back", {}},
        {"First ", Scenario1Strategy::FIRST},
        {"Second ", Scenario1Strategy::SECOND},
        {}
    });

    if (!selection.has_value()) // Error while getting option
        return;

    if (!selection.value().has_value()) { // User wants to go back
        currentMenu = Menu::CHOOSE_SCENARIO;
        return;
    }

    result = scenario1(dataset, selection.value().value());

    currentMenu = Menu::RESULTS;
}

void UserInterface::scenarioTwoMenu(Dataset &dataset) {
    auto selection = optionsMenu<std::optional<Scenario2Strategy>>({
        {"Go back", {}},
        {"First", Scenario2Strategy::FIRST},
        {"Second", Scenario2Strategy::SECOND},
        {"Third", Scenario2Strategy::THIRD},
        {"Fourth", Scenario2Strategy::FOURTH},
        {"Fifth", Scenario2Strategy::FIFTH},
    });

    if (!selection.has_value()) // Error while getting option
        return;

    if (!selection.value().has_value()) { // User wants to go back
        currentMenu = Menu::CHOOSE_SCENARIO;
        return;
    }

    result = scenario2(dataset, selection.value().value());
    currentMenu = Menu::RESULTS;
}

void UserInterface::allScenariosMenu() {
    runAllScenarios();

    getStringInput("Press enter to continue ");
    currentMenu = Menu::MAIN;
}

void UserInterface::resultsMenu() {
    std::cout << "Flow = " << result.flow << "\n";
    std::cout << "Max Capacity = " << result.maxCapacity << "\n";
    std::cout << "Group size = " << result.groupSize << "\n";
    std::cout << "Minimum duration = " << result.minDuration << "\n";

    std::vector<std::list<int>> paths = result.paths;

    for(unsigned i = 0; i < paths.size(); ++i) {
        std::list<int> path = paths.at(i);
        std::cout << "Path nr." << i + 1 << std::endl << std::endl;
        for(unsigned j = 0; j < paths.at(i).size(); ++j) {
            if(j < paths.at(i).size() - 1)
                std::cout << path.front() << " > ";
            else
                std::cout << path.front();
            path.pop_front();
        } 
        std::cout << std::endl << std::endl;
    }

    auto menu = optionsMenu<Menu>({
        {"Continue", Menu::MAIN},
    });
    currentMenu = menu.value_or(currentMenu);
}
