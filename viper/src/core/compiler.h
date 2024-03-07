#pragma once

#include "defines.h"
#include "file.h"
#include <vector>

namespace viper {

/* Compiler options to specify actions for viperc to take */
enum vcompiler_options {
};

/// @brief Run the viper compiler
/// @param files List of files to compile
void run_viperc(std::vector<VFile*> files, i32 option_flags);

}
