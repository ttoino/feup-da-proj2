#ifndef DA_PROJ2_UI_H
#define DA_PROJ2_UI_H

class UserInterface;

#include <limits>
#include <optional>
#include <string>
#include <vector>

#include "dataset.hpp"
#include "scenarios.hpp"

/**
 * @brief Holds the possible menu values.
 */
enum class Menu {
    /** @brief Displays an initial menu to start the program. */
    MAIN,
    /** @brief Allows the user to choose which scenario to run. */
    CHOOSE_SCENARIO,
    /** @brief Allows the user to choose which dataset to use. */
    CHOOSE_DATASET,
    /** @brief Allows the user to generate a new dataset. */
    GENERATE_DATASET,

    VISUALIZE_DATASET,

    /** @brief Runs scenario 1. */
    SCENARIO_ONE,

    SCENARIO_1_1,
    SCENARIO_1_2,

    /** @brief Runs scenario 2. */
    SCENARIO_TWO,

    SCENARIO_2_1,
    SCENARIO_2_2,
    SCENARIO_2_3,
    SCENARIO_2_4,
    SCENARIO_2_5,

    /** @brief Runs all scenarios. */
    ALL_SCENARIOS,

    /** @brief Exits the program. */
    EXIT
};

/**
 * @brief Implements the terminal interface for the user to interact with the
 *        application.
 */
class UserInterface {
    /**
     * @brief Represents the options a user can select in optionsMenu().
     */
    template <class T>
    using Options = std::vector<std::pair<const std::string, T>>;

    /**
     * @brief Specifies which menu to show.
     */
    Menu currentMenu{Menu::MAIN};

    /**
     * @brief The error message to show.
     */
    std::string errorMessage{};

    /**
     * @brief Helper method to show a menu with options.
     *
     * @details Each option string will be shown along with its index on the
     *          list. When the user inputs one of the indices of the list
     *          that option will be returned.
     *
     * @note The first option on the list will be shown last and is
     *       intended to be "Cancel" or "Go back" option.
     *
     * @param options The list of options to show.
     *
     * @return The selected option.
     * @return Nothing if an input error ocurred.
     */
    template <class T> std::optional<T> optionsMenu(const Options<T> &options);

    /**
     * @brief Tries to transform a string into an unsigned integer, displaying
     * an error message if it fails.
     *
     * @note There are optional parameters (min and max) to also display an
     * error message if the prompt is outside of the designated limit.
     *
     * @note Also shows #errorMessage and then resets it.
     *
     * @param prompt Shown to the user.
     * @param min The left bound of the limit (inclusive).
     * @param max The right bound of the limit (inclusive).
     *
     * @return The user input, as an unsigned integer.
     */
    unsigned long getUnsignedInput(
        const std::string &prompt,
        unsigned long min = std::numeric_limits<unsigned long>::min(),
        unsigned long max = std::numeric_limits<unsigned long>::max());

    /**
     * @brief Tries to transform a string into a double, displaying an error
     *        message if it fails.
     *
     * @note There are optional parameters (min and max) to also display an
     * error message if the prompt is outside of the designated limit.
     *
     * @note Also shows #errorMessage and then resets it.
     *
     * @param prompt Shown to the user.
     * @param min The left bound of the limit (inclusive).
     * @param max The right bound of the limit (inclusive).
     *
     * @return The user input, as an unsigned integer.
     */
    double getDoubleInput(const std::string &prompt,
                          double min = std::numeric_limits<double>::min(),
                          double max = std::numeric_limits<double>::max());

    /**
     * @brief Gets a line from stdin and normalizes it.
     *
     * @note Also shows #errorMessage and then resets it.
     *
     * @throws Exit if the EOF bit is set.
     *
     * @param prompt Shown to the user.
     * @return The user input.
     */
    std::string getStringInput(const std::string &prompt);

    /**
     * @brief Checks if an unsigned integer \p n is inside the range [\p min,
     *        \p max].
     *
     * @param n The number to be checked.
     * @param min The left bound of the limit.
     * @param max The right bound of the limit.
     *
     * @return Whether \p n is inside the given range.
     */
    bool inRange(unsigned long n, unsigned long min, unsigned long max);

    /**
     * @brief Checks if a double \p n is inside the range [\p min, \p max].
     *
     * @param n The number to be checked.
     * @param min The left bound of the limit.
     * @param max The right bound of the limit.
     *
     * @return Whether \p n is inside the given range.
     */
    bool inRange(double n, double min, double max);

    /**
     * @brief Helper method to show a list a small amount of items at a time.
     *
     * @details Shows ::ITEMS_PER_PAGE items per page.
     *
     * @tparam T The type of item to show. Must be printable.
     *
     * @param items The list of items to show.
     */
    template <class T> void paginatedMenu(const std::vector<T> &items);

    /**
     * @brief Displays an initial menu to start the program.
     */
    void mainMenu();

    /**
     * @brief Allows the user to choose which scenario to run.
     */
    void chooseScenarioMenu();

    /**
     * @brief Allows the user to choose which dataset to use.
     *
     * @param dataset Where the selected dataset will be stored.
     */
    void chooseDatasetMenu(Dataset &dataset);

    /**
     * @brief Allows the user to generate a new dataset.
     *
     * @param dataset Where the generated dataset will be stored.
     */
    void generateDatasetMenu(Dataset &dataset);

    /**
     * @brief Runs scenario 1.
     *
     * @param dataset The dataset to use
     */
    void scenarioOneMenu(Dataset &dataset);
    
    void scenario1_1Menu(Dataset &dataset);
    void scenario1_2Menu(Dataset &dataset);

    /**
     * @brief Runs scenario 2.
     *
     * @param dataset The dataset to use
     */
    void scenarioTwoMenu(Dataset &dataset);
    
    void scenario2_1Menu(Dataset &dataset);
    void scenario2_2Menu(Dataset &dataset);
    void scenario2_3Menu(Dataset &dataset);
    void scenario2_4Menu(Dataset &dataset);
    void scenario2_5Menu(Dataset &dataset);

    /**
     * @brief Runs all scenarios.
     */
    void allScenariosMenu();

    void visualizeDatasetMenu(Dataset &dataset);

public:
    /**
     * @brief Shows the current menu.
     *
     * @param dataset The currently selected dataset.
     */
    void show(Dataset &dataset);
    /**
     * @brief Shows a message before the program exits.
     */
    void exit();
};

#endif // DA_PROJ2_UI_H
