#pragma once

#include "token.h"
#include "core/file.h"
#include <vector>

namespace viper {

namespace tokenizer {

std::vector<token> tokenize_file(core::VFile* file);

} // tokenizer namespace 
} // viper namespace
