#include <tokenizer/tokenizer.h>
#include "tokenizer_test.h"


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
        viper::token_kind::TK_TYPESPEC_I32,
        viper::token_kind::TK_LSQUIRLY,
        viper::token_kind::TK_LET,
        viper::token_kind::TK_IDENT,
        viper::token_kind::TK_COLON,
        viper::token_kind::TK_TYPESPEC_U32,
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
    viper::VFile file;
    file.name = "test.viper";
    file.content = input;
    std::vector<viper::token> tokens = viper::tokenizer::tokenize_file(&file);

    for (std::size_t i = 0; i < tokens.size(); i++) {
        if (tokens[i].kind != expected[i]) {
            std::printf("test_lexer: expected '%s' and got '%s'\n", 
                    viper::token::kind_to_str(expected[i]).c_str(),
                    viper::token::kind_to_str(tokens[i].kind).c_str());
            result = false;
        }
    }
   
    return result;
}

/// @brief Register 
void tokenizer_register_tests(TestManager &manager) {
    manager.register_test(lexer_test_main_proc, "tokenize basic main procedure");
}
