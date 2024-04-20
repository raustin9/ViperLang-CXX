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

uint8_t parser_struct_basic() {
    bool result = true;
    
    viper::VFile* file = viper::VFile::create_new_ptr();
    file->name = "test.viper";
    file->content = 
        "struct test_struct {\n"
        "   name :: i32;\n"
        "   proc test(): i32 {\n"
        "       let i: i32 = 0;\n"
        "   }\n"
        "}\n"
        ;

    viper::Tokenizer lexer = viper::Tokenizer::create_new(file);
    viper::Parser parser = viper::Parser::create_new(&lexer);

    auto ast = parser.parse();
    ast->print_tree();

    return result;
}

uint8_t parser_return_basic() {
    std::printf("RETURN BASIC\n");
    bool result = true;
    
    viper::VFile* file = viper::VFile::create_new_ptr();
    file->name = "test.viper";
    file->content = "proc main(param1: i32, param2: f32): i32 {\n"
                    "   let i: i32 = 5.0;\n"
                    "   return i + 2 * i;\n"
                    "}\n"
                    ;

    viper::Tokenizer lexer = viper::Tokenizer::create_new(file);
    viper::Parser parser = viper::Parser::create_new(&lexer);

    auto ast = parser.parse();
    ast->print_tree();

    return result;
}


uint8_t parser_conditionals() {
    bool result = true;
    
    viper::VFile* file = viper::VFile::create_new_ptr();
    file->name = "test.viper";
    file->content = "proc main(param1: i32, param2: f32): i32 {\n"
                    "   let i: i32 = 5.0;\n"
                    "   if 1 + 2 {\n"
                    "       let u: i32 = 0 + i;\n"
                    "   } elif i + 5 {\n"
                    "       let v: i32 = 5;\n"
                    "   } else {\n"
                    "       let k: i32 = 3;\n"
                    "       let t: i32 = k + 1;\n"
                    "   }\n"
                    "}\n"
                    ;

    viper::Tokenizer lexer = viper::Tokenizer::create_new(file);
    viper::Parser parser = viper::Parser::create_new(&lexer);

    auto ast = parser.parse();
    ast->print_tree();

    return result;
}

uint8_t parser_for_loop() {
    bool result = true;
    
    viper::VFile* file = viper::VFile::create_new_ptr();
    file->name = "test.viper";
    file->content = "proc main(param1: i32, param2: f32): i32 {\n"
                    "   let i: i32 = 5.0;\n"
                    "   for (let i: i32 = 0; i < 5; i += 1 * 3) {\n"
                    "       let u: i32 = 0 + i;\n"
                    "   }\n"
                    "}\n"
                    ;

    viper::Tokenizer lexer = viper::Tokenizer::create_new(file);
    viper::Parser parser = viper::Parser::create_new(&lexer);

    auto ast = parser.parse();
    ast->print_tree();

    return result;
}


uint8_t parser_while_loop() {
    bool result = true;
    
    viper::VFile* file = viper::VFile::create_new_ptr();
    file->name = "test.viper";
    file->content = "proc main(param1: i32, param2: f32): i32 {\n"
                    "   let i: i32 = 5.0;\n"
                    "   while (i + 2) {\n"
                    "       let i: i32 = 5 + 1;\n"
                    "       if (i + 2) {\n"
                    "           let u: i32 = 5;\n"
                    "       }\n"
                    "   }\n"
                    "}\n"
                    ;

    viper::Tokenizer lexer = viper::Tokenizer::create_new(file);
    viper::Parser parser = viper::Parser::create_new(&lexer);

    auto ast = parser.parse();
    ast->print_tree();

    return result;
}


uint8_t parser_do_while_loop() {
    bool result = true;
    std::printf("DO WHILE\n");
    
    viper::VFile* file = viper::VFile::create_new_ptr();
    file->name = "test.viper";
    file->content = "proc main(param1: i32, param2: f32): i32 {\n"
                    "   let i: i32 = 5.0;\n"
                    "   do {\n"
                    "       let i: i32 = 5 + 1;\n"
                    "       if (i + 2) {\n"
                    "           let u: i32 = 5;\n"
                    "       }\n"
                    "   } while (i + 2);\n"
                    "}\n"
                    ;

    viper::Tokenizer lexer = viper::Tokenizer::create_new(file);
    viper::Parser parser = viper::Parser::create_new(&lexer);

    auto ast = parser.parse();
    ast->print_tree();

    return result;
}


uint8_t parser_test_custom_typespec() {
    std::printf("CUSTOM TYPESPEC\n");
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

uint8_t parser_test_member_access_expression() {
    bool result = true;
    
    viper::VFile* file = viper::VFile::create_new_ptr();
    file->name = "test.viper";
    file->content = "let i: i32 = num_seconds.field;\n"
                    "let i: i32 = num_milliseconds.method();\n"
                    "let i: i32 = test_struct.method(1 + 2, num_seconds);\n";

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
    manager.register_test(parser_conditionals, "Test parsing of conditional if-elif-else chain");
    manager.register_test(parser_for_loop, "Test parsing of for loop");
    manager.register_test(parser_while_loop, "Test parsing of while loops");
    manager.register_test(parser_do_while_loop, "Test parsing of do-while loops");
    manager.register_test(parser_struct_basic, "Test parsing of simple struct definition");
    manager.register_test(parser_return_basic, "Test the parsing of a simple return statement from a function");
    manager.register_test(parser_test_custom_typespec, "Test using identifier for type specifier");
    manager.register_test(parser_test_let, "Test top level var declarations");
    manager.register_test(parser_test_expression, "Test basic single-expression parsing");
    manager.register_test(parser_test_identifier_expression, "Test basic identifier expression parsing");
    manager.register_test(parser_test_expression_chain, "Test expression chain parsing");
    manager.register_test(parser_test_prefix, "Test parsing expressions with prefix operators");
    manager.register_test(parser_test_str, "Test parsing string literal expressions");
    manager.register_test(parser_test_grouping_expression, "Test basic grouping expression parsing");
    manager.register_test(parser_test_identifier_dimension_expression, "Test basic identifier dimension access expression parsing");
    manager.register_test(parser_test_member_access_expression, "Test member access expression parsing");
}
