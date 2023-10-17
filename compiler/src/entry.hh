#include "defines.hh"

#include "preprocessor/preprocessor.hh"
#include "lexer/lexer.hh"

#include "core/file.hh"
#include "core/token.hh"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

// Test function to test preprocessor output
HAPI std::string test_preprocessor(char* source_name);

// Test function to test lexer output
HAPI std::vector<token_t> test_lexer(char* source_name);
