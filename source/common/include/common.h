#pragma once

#include <fstream>
#include <iostream>
#include <sstream>

#define LOG_STDERR(x) std::cerr << x << std::endl
#define LOG_STDOUT(x) std::cout << x << std::endl

#define GETSET_MEMBER(type, name)\
 inline const type Get##name() { return m##name; }\
 inline void Set##name(##const type value) { m##name = value; }

#define GETSET_ARRAY_AT(type, arr, index, name)\
 inline const type Get##name() { return arr.at(index); }\
 inline void Set##name(##const type value) { arr.at(index) = value; }

namespace Vision
{
namespace Util
{

 /**
 * @brief Class representing a string obtained by reading a text file.
 *
 * This class is derived from std::string and provides functionality to read a text file
 * from the provided path and initialize the string with the contents of that file.
 */
class FileString : public std::string
{
public:
    /**
     * @brief Constructor that reads a text file from the provided path and initializes the string.
     *
     * @param path The path to the text file.
     */
    FileString(const char* path);
    /**
     * @brief Method to change the file by providing a new path.
     *
     * This method reads a new text file from the provided path and updates the string with
     * the contents of the new file.
     *
     * @param path The new path to the text file.
     */
    void ChangeFile(const char* path);
};

/**
 * @brief Template function to retrieve the size of a raw array.
 *
 * This function takes a raw array as a parameter and returns its size.
 *
 * @tparam T The type of the array elements.
 * @tparam N The size of the array.
 * @param[in] arr The input raw array.
 * @return The size of the array.
 */
template <typename T, size_t N>
inline const size_t RawArraySize(const T(&)[N])
{
    return N;
}

/**
 * @brief Template function to retrieve the byte size of a raw array.
 *
 * This function takes a raw array as a parameter and returns its byte size.
 *
 * @tparam T The type of the array elements.
 * @tparam N The size of the array.
 * @param[in] arr The input raw array.
 * @return The byte size of the array.
 */
template <typename T, size_t N>
inline const size_t RawArrayByteSize(const T(&)[N])
{
    return N * sizeof(T);
}

} // namespace System
} // namespace Vision