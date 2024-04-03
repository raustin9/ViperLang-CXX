#include <parser/parser.h>
#include <tokenizer/tokenizer.h>
#include "parser_test.h"
#include "token.h"


// Test the tokenizer
uint8_t parser_test_basic() {
    bool result = true;
    
    viper::VFile* file = viper::VFile::create_new_ptr();
    file->name = "test.viper";
    file->content = "proc main(param1: i32, param2: f32): i32 {\n"
                    "   let i: i32 = 5.0;\n"
                    "}\n"
                    ;

    viper::Tokenizer lexer = viper::Tokenizer::create_new(file);
    viper::Parser parser = viper::Parser::create_new(&lexer);

    auto ast = parser.parse();

    return result;
}

uint8_t parser_test_let() {
    bool result = true;
    
    viper::VFile* file = viper::VFile::create_new_ptr();
    file->name = "test.viper";
    file->content = "let x: i32 = 0; let y: f32 = 9;";

    viper::Tokenizer lexer = viper::Tokenizer::create_new(file);
    viper::Parser parser = viper::Parser::create_new(&lexer);

    auto ast = parser.parse();

    return result;
}

/// @brief Register 
void parser_register_tests(TestManager &manager) {
    manager.register_test(parser_test_basic, "Test simple parser behavior");
    manager.register_test(parser_test_let, "Test top level var declarations");
}
