#pragma once

#include "defines.h"

#include <string>

namespace core {
    
// Source codes files
struct VFile {
    std::string name;
    i32 file_number;
    std::string content;

    // From chibic project. Maybe use?
    std::string display_name;
    i32 line_delta;

    static VFile from(const std::string& file_path);
};

}
