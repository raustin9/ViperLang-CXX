#pragma once

/*
 * Lexer
 * The lexer takes in code (text) as input and transforms it into a Token Stream
 * that it then outputs which the parser will normally take from there.
 */

#include "core/token.hh"
#include <string>
#include <vector>
#include <map>

class Lexer {
    public:
        Lexer(const std::string& source_code);
        ~Lexer();
        Lexer(const Lexer&) = delete;
        Lexer& operator= (const Lexer&) = delete;

        // Member functions
        void tokenize_input();

        // Accessors
        std::vector<token_t> get_token_stream() { return _token_stream; }

    private:
        const std::string _source_code;
        std::vector<token_t> _token_stream;
        std::map <std::string, token_e> _keywords;
        uint64_t _line_num = 1;
        size_t _read_position = 0;
        size_t _position = 0;
        char _cur_char;

        void _skip_whitespace();
        char _peek_char();
        std::string _read_identifier();
        token_e _lookup_identifier(std::string ident);
        token_t _read_number();
        token_t _get_next_token();
        void _read_char();
};
