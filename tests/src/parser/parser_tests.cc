#include <parser/parser.h>
#include <tokenizer/tokenizer.h>
#include "parser_test.h"
#include "token.h"


// Test the tokenizer
uint8_t parser_test_basic() {
    bool result = true;
    
    viper::VFile* file = viper::VFile::create_new_ptr();
    file->name = "test.viper";
    file->content = "proc main(): i32 {}\n";

    viper::Tokenizer lexer = viper::Tokenizer::create_new(file);
    viper::Parser parser = viper::Parser::create_new(&lexer);

    auto ast = parser.parse();

    return result;
}

/// @brief Register 
void parser_register_tests(TestManager &manager) {
    manager.register_test(parser_test_basic, "Test simple parser behavior");
}
