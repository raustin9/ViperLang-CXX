#pragma once

#include "token.h"
#include "core/viper.h"
#include <unordered_map>
#include <list>

namespace viper {

class Tokenizer {
    public:
        ~Tokenizer() {}

        static Tokenizer create_new(VFile* file);
        std::list<token> tokenize_file();


    private:
        Tokenizer() {}
        Tokenizer(Tokenizer& t) {
            position = t.position;
            line_num = t.line_num;
            read_position = t.read_position;
            current_char = t.current_char;
            m_input_ptr = t.m_input_ptr;
            // m_file = std::move(t.m_file);
            m_file = t.m_file;
        }

        void canonicalize_newline();
        void read_char();
        void skip_whitespace();
        token read_number();
        char peek_char();
        token_kind lookup_identifier(const std::string& identifier);
        std::string read_identifier();
        void skip_single_line_comment();
        void skip_multi_line_comment();
        std::string read_string_content();
        token next_token();
        void tokenize();


        // std::unique_ptr<VFile> m_file;
        VFile* m_file;                                          // [NOT OWNED] Pointer to the file's content we are tokenizing
        std::unordered_map<std::string, token_kind> keywords;   // Keywords with corresponding tokens
        std::list<token> tokens;
        u32 line_num = 0;                                       // Line number of the file we are currently on
        u64 position = 0;                                       
        u64 read_position = 0;
        char current_char;                                      // Current character of the source  code
        std::string* m_input_ptr;                               // [NOT OWNED] The source code input we are tokenizing
};
} // viper namespace
