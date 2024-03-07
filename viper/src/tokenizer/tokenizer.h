#pragma once

#include "token.h"
#include "core/file.h"
#include <vector>

namespace viper {

namespace tokenizer {

std::vector<token> tokenize_file(viper::VFile* file);

} // tokenizer namespace 
} // viper namespace
