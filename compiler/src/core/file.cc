#include "core/file.hh"
#include <fstream>
#include <sstream>

std::string
read_file(char* file_path) {
    std::ifstream fin(file_path);

    std::stringstream buffer;
    buffer << fin.rdbuf();
    std::string rv = buffer.str();

    return rv;
}
