#ifndef DA_PROJ2_CONSTANTS_H
#define DA_PROJ2_CONSTANTS_H

#include <string>

/**
 * @brief Where the different datasets are stored.
 */
const std::string DATASETS_PATH = "datasets/";
/**
 * @brief Where the scenario results will be stored in the datasets folder.
 */
const std::string OUTPUT_FILE = "output.csv";
/**
 * @brief The header to be printed at the start of the scenario results.
 */
const std::string OUTPUT_HEADER = "dataset,scenario,heuristic,runtime,maxFlow,maxCapacity,path,minDuration\n";
/**
 * @brief How many items to print in each page in UserInterface#paginatedMenu().
 */
const unsigned int ITEMS_PER_PAGE = 10;

// Terminal escape sequences
#ifdef NO_ANSI

const struct ClearScreen {
    friend std::ostream &operator<<(std::ostream &out, const ClearScreen &c) {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        return out;
    }
} CLEAR_SCREEN;

const std::string RED_TEXT = "";
const std::string GREEN_TEXT = "";
const std::string CONCEAL_TEXT = "";
const std::string STRIKE_THROUGH = "";
const std::string DISABLE_CURSOR = "";
const std::string ENABLE_CURSOR = "";
const std::string RESET_FORMATTING = "";
#else
/**
 * @brief Clears the screen.
 *
 * @note May be implemented as an escape sequence or as a system call.
 */
const std::string CLEAR_SCREEN = "\33[2J\33[H";
/**
 * @brief Turns any text that follows red.
 *
 * @note May be disabled.
 */
const std::string RED_TEXT = "\33[31m";
/**
 * @brief Turns any text that follows green.
 *
 * @note May be disabled.
 */
const std::string GREEN_TEXT = "\33[32m";
/**
 * @brief Hides any text that follows.
 *
 * @note May be disabled.
 */
const std::string CONCEAL_TEXT = "\33[8m";
/**
 * @brief Crosses out any text that follows.
 *
 * @note May be disabled.
 */
const std::string STRIKE_THROUGH = "\33[9m";
/**
 * @brief Disables the terminal cursor.
 *
 * @note May be disabled.
 */
const std::string DISABLE_CURSOR = "\33[?25l";
/**
 * @brief Enables the terminal cursor.
 *
 * @note May be disabled.
 */
const std::string ENABLE_CURSOR = "\33[?25h";
/**
 * @brief Resets any text that follows back to normal.
 *
 * @note May be disabled.
 */
const std::string RESET_FORMATTING = "\33[0m";
#endif // NO_ANSI

/**
 * @brief This program's name.
 */
const std::string PROGRAM_NAME = "DA - Project 2";

#endif // DA_PROJ2_CONSTANTS_H
