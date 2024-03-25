#include <tokenizer/tokenizer.h>
#include "tokenizer_test.h"
#include <list>


// Test the tokenizer
uint8_t lexer_test_main_proc() {
    std::string input = "proc main(): i32 {"
        "   let x: u32 = 1;"
        "   return 0;"
        "}";
    
    std::vector<viper::token_kind> expected = {
        viper::token_kind::TK_PROC,
        viper::token_kind::TK_IDENT,
        viper::token_kind::TK_LPAREN,
        viper::token_kind::TK_RPAREN,
        viper::token_kind::TK_COLON,
        // viper::token_kind::TK_TYPESPEC_I32,
        viper::token_kind::TK_IDENT,
        viper::token_kind::TK_LSQUIRLY,
        viper::token_kind::TK_LET,
        viper::token_kind::TK_IDENT,
        viper::token_kind::TK_COLON,
        // viper::token_kind::TK_TYPESPEC_U32,
        viper::token_kind::TK_IDENT,
        viper::token_kind::TK_ASSIGN,
        viper::token_kind::TK_NUM_INT,
        viper::token_kind::TK_SEMICOLON,
        viper::token_kind::TK_RETURN,
        viper::token_kind::TK_NUM_INT,
        viper::token_kind::TK_SEMICOLON,
        viper::token_kind::TK_RSQUIRLY,
        viper::token_kind::TK_EOF
    };

    bool result = true;
    viper::VFile* file = viper::VFile::create_new_ptr();
    file->name = "test.viper";
    file->content = input;

    viper::Tokenizer tokenizer = viper::Tokenizer::create_new(file);
    // std::vector<viper::token> tokens = tokenizer.tokenize_file();

    for (std::size_t i = 0; i < expected.size(); i++) {
        viper::token tok = tokenizer.next_token();
        if (tok.kind != expected[i]) {
            std::printf("test_lexer: expected '%s' and got '%s'\n", 
                    viper::token::kind_to_str(expected[i]).c_str(),
                    viper::token::kind_to_str(tok.kind).c_str());
            result = false;
            return result;
        }
    }
   
    return result;
}

uint8_t lexer_test_string_literals() {
    std::string input = "proc main(): i32 {"
        "   let test: u8 = \"test string content\";"
        "   return 0;"
        "}";
    
    std::vector<viper::token_kind> expected = {
    // std::vector<viper::token_kind> expected = {
        viper::token_kind::TK_PROC,
        viper::token_kind::TK_IDENT,
        viper::token_kind::TK_LPAREN,
        viper::token_kind::TK_RPAREN,
        viper::token_kind::TK_COLON,
        // viper::token_kind::TK_TYPESPEC_I32,
        viper::token_kind::TK_IDENT,
        viper::token_kind::TK_LSQUIRLY,

        viper::token_kind::TK_LET,
        viper::token_kind::TK_IDENT,
        viper::token_kind::TK_COLON,
        // viper::token_kind::TK_TYPESPEC_U8,
        viper::token_kind::TK_IDENT,
        viper::token_kind::TK_ASSIGN,
        viper::token_kind::TK_STR,
        viper::token_kind::TK_SEMICOLON,

        viper::token_kind::TK_RETURN,
        viper::token_kind::TK_NUM_INT,
        viper::token_kind::TK_SEMICOLON,
        viper::token_kind::TK_RSQUIRLY,
        viper::token_kind::TK_EOF
    };

    bool result = true;
    viper::VFile* file = viper::VFile::create_new_ptr();
    file->name = "test.viper";
    file->content = input;

    viper::Tokenizer tokenizer = viper::Tokenizer::create_new(file);
    // std::vector<viper::token> tokens = tokenizer.tokenize_file();
    // std::vector<viper::token> tokens = tokenizer.tokenize_file();

    for (std::size_t i = 0; i < expected.size(); i++) {
        viper::token tok = tokenizer.next_token();
        if (tok.kind != expected[i]) {
            std::printf("test_lexer_basic_str: expected '%s' and got '%s'\n", 
                    viper::token::kind_to_str(expected[i]).c_str(),
                    viper::token::kind_to_str(tok.kind).c_str());
            result = false;
            return result;
        }

        if (tok.kind == viper::TK_STR && tok.name != "test string content") {
            return false;
        }
    }
   
    return result;
}

/// @brief Register 
void tokenizer_register_tests(TestManager &manager) {
    manager.register_test(lexer_test_string_literals, "test tokenizing basic string literal");
    manager.register_test(lexer_test_main_proc, "tokenize basic main procedure");
}
