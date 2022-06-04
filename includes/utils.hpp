#ifndef DA_PROJ2_UTILS_H
#define DA_PROJ2_UTILS_H

/**
 * @brief Declares a scoped enumeration type.
 *
 * @details Enumerations declared by this macro will have two additional
 *          enumerators, BEGIN and END, at the beginning and at the end of the
 *          enumerator list, respectively. These allow looping through the
 *          enumerators by using #FOR_ENUM like a regular for loop.
 *
 * @param name The name of the enum.
 * @param ... The enumerators
 */
#define ENUM(name, ...)                                                        \
    enum class name {                                                          \
        /**                                                                    \
         * @brief Special enumerator that marks the beginning of the enum.     \
         *                                                                     \
         * @warning Should not be used as an actual value.                     \
         */                                                                    \
        BEGIN,                                                                 \
        __VA_ARGS__,                                                           \
        /**                                                                    \
         * @brief Special enumerator that marks the end of the enum.           \
         *                                                                     \
         * @warning Should not be used as an actual value.                     \
         */                                                                    \
        END,                                                                   \
    }

/**
 * @brief Loops through the enumerators of an enumeration type.
 *
 * @param type The enum type. Must have been declared with #ENUM.
 * @param name The name of the enum variable.
 */
#define FOR_ENUM(type, name)                                                   \
    for (type name = static_cast<type>(static_cast<int>(type::BEGIN) + 1);     \
         name != type::END;                                                    \
         name = static_cast<type>(static_cast<int>(name) + 1))

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
