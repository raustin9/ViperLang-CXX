#pragma once

#include <vector>
#include <string>

namespace mantis {

/// @brief Split a string into a vector of strings separated by the delimiter
std::vector<std::string> split_str(std::string str, std::string delimiter);

}
