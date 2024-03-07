#pragma once

#include "defines.h"

#include <string>

namespace viper {
    
// Source codes files
struct VFile {
    VFile() {}
    std::string name;
    i32 file_number;
    std::string content;

    // From chibic project. Maybe use?
    std::string display_name;
    i32 line_delta;

    static VFile from(const std::string& file_path);
};

}
