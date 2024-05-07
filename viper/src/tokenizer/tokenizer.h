#pragma once

#include "token.h"
#include "core/core.h"
#include <unordered_map>
#include <vector>

namespace viper {

class Tokenizer {
    public:
        ~Tokenizer() {}
        Tokenizer(Tokenizer&&) = default;
        Tokenizer(const Tokenizer& tok) = default;

        static Tokenizer create_new(VFile* file);
        std::vector<token> tokenize_file();

        token next_token(); // Get the next token from the input source
        token peek_token(); // Peek a token without advancing through the code

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
        token skip_single_line_comment();
        token skip_multi_line_comment();
        std::string read_string_content();
        void tokenize();


        // std::unique_ptr<VFile> m_file;
        VFile* m_file;                                          // [NOT OWNED] Pointer to the file's content we are tokenizing
        std::unordered_map<std::string, token_kind> keywords;   // Keywords with corresponding tokens
        std::vector<token> tokens;
        u32 line_num = 0;                                       // Line number of the file we are currently on
        u64 position = 0;                                       
        u64 read_position = 0;
        char current_char;                                      // Current character of the source  code
        std::string* m_input_ptr;                               // [NOT OWNED] The source code input we are tokenizing
};
} // viper namespace
