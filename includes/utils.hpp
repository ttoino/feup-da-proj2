#ifndef DA_PROJ2_UTILS_H
#define DA_PROJ2_UTILS_H

#include <string>
#include <vector>

/**
 * @brief Exception thrown when the user wants to exit the program.
 */
class Exit {};

/**
 * @brief Splits a string by a character.
 *
 * @param str The string to be split.
 * @param sep The character to use as the separator.
 * @returns A vector with all the words in the string that were separated by the
 *         given character.
 */
std::vector<std::string> split(std::string str, char sep);

/**
 * @brief Normalizes a string.
 *
 * @details Replaces all tabs with spaces, removes duplicate spaces and trims
 *          spaces from the start and the end.
 *
 * @param input The string to normalize.
 */
void normalizeInput(std::string &input);

#endif
