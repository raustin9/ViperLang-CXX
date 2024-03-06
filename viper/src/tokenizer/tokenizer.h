#pragma once

#include "viper.h"
#include <vector>

namespace viper {

namespace tokenizer {

std::vector<token> tokenize_file(VFile* file);

} // tokenizer namespace 
} // viper namespace
