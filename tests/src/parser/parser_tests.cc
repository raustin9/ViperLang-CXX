#include <cstdint>
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
    ast->print_tree();

    return result;
}

uint8_t parser_test_custom_typespec() {
    bool result = true;
    
    viper::VFile* file = viper::VFile::create_new_ptr();
    file->name = "test.viper";
    file->content = "proc main(param1: User, param2: f32): i32 {\n"
                    "   let i: i32 = 5.0;\n"
                    "}\n"
                    ;

    viper::Tokenizer lexer = viper::Tokenizer::create_new(file);
    viper::Parser parser = viper::Parser::create_new(&lexer);

    auto ast = parser.parse();
    ast->print_tree();

    return result;
}

uint8_t parser_test_prefix() {
    bool result = true;

    viper::VFile* file = viper::VFile::create_new_ptr();
    file->name = "test.viper";
    file->content = 
        "let i: i32 = -5.0;\n";

    viper::Tokenizer lexer = viper::Tokenizer::create_new(file);
    viper::Parser parser = viper::Parser::create_new(&lexer);

    auto ast = parser.parse();
    ast->print_tree();

    return result;
}

uint8_t parser_test_let() {
    bool result = true;
    
    viper::VFile* file = viper::VFile::create_new_ptr();
    file->name = "test.viper";
    file->content = "let x: i32 = 2; let y: f32 = 9.0;";

    viper::Tokenizer lexer = viper::Tokenizer::create_new(file);
    viper::Parser parser = viper::Parser::create_new(&lexer);

    auto ast = parser.parse();
    ast->print_tree();

    return result;
}

uint8_t parser_test_str() {
    bool result = true;
    
    viper::VFile* file = viper::VFile::create_new_ptr();
    file->name = "test.viper";
    file->content = "let x: str = \"test string\";";

    viper::Tokenizer lexer = viper::Tokenizer::create_new(file);
    viper::Parser parser = viper::Parser::create_new(&lexer);

    auto ast = parser.parse();
    ast->print_tree();

    return result;
}

uint8_t parser_test_expression() {
    bool result = true;
    
    viper::VFile* file = viper::VFile::create_new_ptr();
    file->name = "test.viper";
    file->content = "let i: i32 = 1 + 2;\n";

    viper::Tokenizer lexer = viper::Tokenizer::create_new(file);
    viper::Parser parser = viper::Parser::create_new(&lexer);

    auto ast = parser.parse();

    ast->print_tree();

    return result;
}

uint8_t parser_test_expression_chain() {
    bool result = true;
    
    viper::VFile* file = viper::VFile::create_new_ptr();
    file->name = "test.viper";
    file->content = "let i: i32 = 1 + 2 * 3;\n"
                    "let i: i32 = 1 + 2 * 3 + 4;\n"
                    "let i: i32 = 1 + 2 * -3 + 4;\n";

    viper::Tokenizer lexer = viper::Tokenizer::create_new(file);
    viper::Parser parser = viper::Parser::create_new(&lexer);

    auto ast = parser.parse();

    ast->print_tree();

    return result;
}

uint8_t parser_test_grouping_expression() {
    bool result = true;
    
    viper::VFile* file = viper::VFile::create_new_ptr();
    file->name = "test.viper";
    file->content = "let i: i32 = (1 + 2) * 3;\n"
                    "let i: i32 = 1 + 2 * (3 + 4);\n"
                    "let i: i32 = 1 + 2 * -3 + 4;\n";

    viper::Tokenizer lexer = viper::Tokenizer::create_new(file);
    viper::Parser parser = viper::Parser::create_new(&lexer);

    auto ast = parser.parse();

    ast->print_tree();

    return result;
}

uint8_t parser_test_identifier_expression() {
    bool result = true;
    
    viper::VFile* file = viper::VFile::create_new_ptr();
    file->name = "test.viper";
    file->content = "let i: i32 = num_seconds;\n"
                    "let i: i32 = num_milliseconds(num_seconds, 1 + 2 * 3);\n";

    viper::Tokenizer lexer = viper::Tokenizer::create_new(file);
    viper::Parser parser = viper::Parser::create_new(&lexer);

    auto ast = parser.parse();

    ast->print_tree();

    return result;
}

uint8_t parser_test_identifier_dimension_expression() {
    bool result = true;
    
    viper::VFile* file = viper::VFile::create_new_ptr();
    file->name = "test.viper";
    file->content = "let i: i32 = num_seconds[1 + 2];\n"
                    "let i: i32 = num_milliseconds(num_seconds[1 + 2 * 3], 1 + 2 * 3);\n";

    viper::Tokenizer lexer = viper::Tokenizer::create_new(file);
    viper::Parser parser = viper::Parser::create_new(&lexer);

    auto ast = parser.parse();

    ast->print_tree();

    return result;
}

/// @brief Register 
void parser_register_tests(TestManager &manager) {
    manager.register_test(parser_test_basic, "Test simple parser behavior");
    manager.register_test(parser_test_custom_typespec, "Test using identifier for type specifier");
    manager.register_test(parser_test_let, "Test top level var declarations");
    manager.register_test(parser_test_expression, "Test basic single-expression parsing");
    manager.register_test(parser_test_identifier_expression, "Test basic identifier expression parsing");
    manager.register_test(parser_test_expression_chain, "Test expression chain parsing");
    manager.register_test(parser_test_prefix, "Test parsing expressions with prefix operators");
    manager.register_test(parser_test_str, "Test parsing string literal expressions");
    manager.register_test(parser_test_grouping_expression, "Test basic grouping expression parsing");
    manager.register_test(parser_test_identifier_dimension_expression, "Test basic identifier dimension access expression parsing");
}
