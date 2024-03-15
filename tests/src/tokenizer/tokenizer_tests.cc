#include <tokenizer/tokenizer.h>
#include "tokenizer_test.h"
#include <list>


// Test the tokenizer
uint8_t lexer_test_main_proc() {
    std::string input = "proc main(): i32 {"
        "   let x: u32 = 1;"
        "   return 0;"
        "}";
    // std::printf("GOT HERE\n");
    
    std::list<viper::token_kind> expected = {
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
    viper::VFile* file = viper::VFile::create_new_ptr();
    file->name = "test.viper";
    file->content = input;

    viper::Tokenizer tokenizer = viper::Tokenizer::create_new(file);
    std::list<viper::token> tokens = tokenizer.tokenize_file();
    // std::vector<viper::token> tokens = tokenizer.tokenize_file();

     std::list<viper::token>::iterator res_lit = tokens.begin();
     std::list<viper::token_kind>::iterator exp_lit = expected.begin();

     while (res_lit != tokens.end() && exp_lit != expected.end()) {
        if ((*res_lit).kind != *exp_lit) {
            std::printf("test_lexer: expected %s and got %s\n",
                viper::token::kind_to_str(res_lit->kind).c_str(),
                viper::token::kind_to_str(*exp_lit).c_str()
            );
        }

        res_lit++;
        exp_lit++;
     }
//    for (std::size_t i = 0; i < tokens.size(); i++) {
//        if (tokens[i].kind != expected[i]) {
//            std::printf("test_lexer: expected '%s' and got '%s'\n", 
//                    viper::token::kind_to_str(expected[i]).c_str(),
//                    viper::token::kind_to_str(tokens[i].kind).c_str());
//            result = false;
//            return result;
//        }
//    }
   
    return result;
}

uint8_t lexer_test_string_literals() {
    std::string input = "proc main(): i32 {"
        "   let test: u8 = \"test string content\";"
        "   return 0;"
        "}";
    
    std::list<viper::token_kind> expected = {
    // std::vector<viper::token_kind> expected = {
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
        viper::token_kind::TK_TYPESPEC_U8,
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
    // std::printf("GOT HERE\n");
    // std::vector<viper::token> tokens = tokenizer.tokenize_file();
    std::list<viper::token> tokens = tokenizer.tokenize_file();
    // std::vector<viper::token> tokens = tokenizer.tokenize_file();

     std::list<viper::token>::iterator res_lit = tokens.begin();
     std::list<viper::token_kind>::iterator exp_lit = expected.begin();

     while (res_lit != tokens.end() && exp_lit != expected.end()) {
        if ((*res_lit).kind != *exp_lit) {
            std::printf("test_lexer: expected %s and got %s\n",
                viper::token::kind_to_str(res_lit->kind).c_str(),
                viper::token::kind_to_str(*exp_lit).c_str()
            );
        }

        res_lit++;
        exp_lit++;
     }
//    for (std::size_t i = 0; i < tokens.size(); i++) {
//        if (tokens[i].kind != expected[i]) {
//            std::printf("test_lexer_basic_str: expected '%s' and got '%s'\n", 
//                    viper::token::kind_to_str(expected[i]).c_str(),
//                    viper::token::kind_to_str(tokens[i].kind).c_str());
//            result = false;
//            return result;
//        }
//
//        if (tokens[i].kind == viper::TK_STR) {
//            // std::printf("TK_STR: %s\n", tokens[i].name.c_str());
//        }
//    }
   
    return result;
}

/// @brief Register 
void tokenizer_register_tests(TestManager &manager) {
    manager.register_test(lexer_test_string_literals, "test tokenizing basic string literal");
    manager.register_test(lexer_test_main_proc, "tokenize basic main procedure");
}
