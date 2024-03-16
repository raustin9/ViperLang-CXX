#pragma once

#include "defines.h"
#include "viper.h"
#include <vector>
#include <string>

namespace viper {

/* Compiler options to specify actions for viperc to take */
enum vcompiler_options {
};

class ViperC {
    public:
        ViperC() {}
        ~ViperC() {}

        /// TODO: implement this when we get to the point of using the command line
        /// @brief Parse the command line arguments
        /// @returns An integer that is |= with each compiler option flag
        i32 parse_command_line_args(const std::string& args);

        /// TODO: implement this when we are using option flags for the compiler
        /// @brief Run the viper compiler
        /// @param files List of files to compile
        void run_viperc(std::vector<VFile*> files, i32 option_flags);
    private:
};


}
