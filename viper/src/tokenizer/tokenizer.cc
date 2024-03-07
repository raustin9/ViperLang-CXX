#include "tokenizer.h"
#include <cctype>
#include <unordered_map>
#include <vector>
#include <iostream>

namespace viper {

namespace tokenizer {

static u32 line_num = 0;
static u32 position = 0;
static u64 read_position = 0;
static char current_char;

static std::unordered_map<std::string, token_kind> keywords;
static std::vector<token> tokens;
static std::string* input_ptr;
static viper::VFile* file_ptr;


/// @brief Turn all \r or \r\n into just \n
void canonicalize_newline(std::string &s) {
    i32 i = 0, j = 0;

    while (s[i]) {
        if (s[i] == '\r' && s[i+1] == '\n') {
            i += 2;
            s[j++] = '\n';
        } else if (s[i] == '\r') {
            i++;
            s[j++] = '\n';
        } else {
            s[j++] = s[i++];
        }
    }

    s[j] = '\0';
}

/// @brief Eat a character and put it in current_char
static void read_char() {
    if(read_position >= input_ptr->length()) {
        current_char = '\0';
    } else {
        current_char = input_ptr->at(read_position);
    }

    position = read_position;
    read_position++;
}

/// @brief Skip over whitespace characters
static void skip_whitespace() {
    while (current_char == ' ' ||
           current_char == '\n' ||
           current_char == '\t' ||
           current_char == '\r') {
        if (current_char == '\n') {
            line_num++;
        }

        read_char();
    }
}


/// @brief Read a number literal. Either floating point or integer
static token read_number() {
    token tok;
    u64 pos = position;
    bool decimal = false;
    bool is_legal = true;

    while (isdigit(current_char) != 0 || current_char == '.') {
        if (current_char == '.' && decimal == true) {
            is_legal = false;
        }

        if (current_char == '.') decimal = true;
        read_char();
    }

    if (!is_legal) {
        tok.kind = token_kind::TK_ILLEGAL;
    } else if (decimal) {
        tok.kind = token_kind::TK_NUM_FLOAT;
    } else {
        tok.kind = token_kind::TK_NUM_INT;
    }

    tok.name = input_ptr->substr(pos, position-pos);
    return tok;
}


/// @brief Look at the next character in input without incrimenting
static char peek_char() {
    if (read_position >= input_ptr->length()) {
        return '\0';
    }
    
    return input_ptr->at(read_position);
}


/// @brief Lookup an identifier string in the table of keywords, and if 
///        it is not a keyword it is just normal identifier
static token_kind lookup_identifier(const std::string& identifier) {
    if (keywords.find(identifier) != keywords.end()) {
        return keywords[identifier];
    }

    return token_kind::TK_IDENT;
}


/// @brief Read an identifier string
static std::string read_identifier() {
    u32 pos = position;
    while (isalpha(current_char) != 0 ||
           current_char == '_' ||
           isdigit(current_char) != 0) {
        read_char();
    }

    return input_ptr->substr(pos, position-pos);
}


/// @brief Skip over multi line comments
static void skip_single_line_comment() {
    while (current_char != '\n') {
        read_char();
    }
}


/// @brief Skip block comment
static void skip_multi_line_comment() {
    if (input_ptr->find("*/") == std::string::npos) {
        std::cout << "Unclosed block comment";
        return;
    }
    while (1) {
        if (current_char == '*' && peek_char() == '/') {
            read_char();
            break;
        }
    }
}


/// @brief Get the next token and return it
static token next_token() {
    token tok;

    skip_whitespace();

    switch(current_char) {
        case '/':
            if (peek_char() == '/') {
                skip_single_line_comment();
            } else if (peek_char() == '*') {
                skip_multi_line_comment();
            } else if (peek_char() == '=') {
                read_char();
                tok = token::create_new(TK_DIVEQ, "/=", line_num);
            } else {
                tok = token::create_new(TK_SLASH, std::string(1, current_char), line_num);
            }
            break;


        case '=':
            if (peek_char() == '=') {
                read_char(); // eat the next =
                tok = token::create_new(TK_EQUALTO, "==", line_num);
            } else {
                tok = token::create_new(TK_ASSIGN, std::string(1, current_char), line_num);
            }
            break;

        case ';':
            tok = token::create_new(TK_SEMICOLON, std::string(1, current_char), line_num);
            break;

        case '(':
            tok = token::create_new(TK_LPAREN, std::string(1, current_char), line_num);
            break;

        case ')':
            tok = token::create_new(TK_RPAREN, std::string(1, current_char), line_num);
            break;

        case '{':
            tok = token::create_new(TK_LSQUIRLY, std::string(1, current_char), line_num);
            break;

        case '}':
            tok = token::create_new(TK_RSQUIRLY, std::string(1, current_char), line_num);
            break;

        case '[':
            tok = token::create_new(TK_LBRACKET, std::string(1, current_char), line_num);
            break;

        case ']':
            tok = token::create_new(TK_RBRACKET, std::string(1, current_char), line_num);
            break;

        case ',':
            tok = token::create_new(TK_COMMA, std::string(1, current_char), line_num);
            break;
        
        case '.':
            tok = token::create_new(TK_DOT, std::string(1, current_char), line_num);
            break;
        
        case '+':
            if (peek_char() == '=') {
                read_char();
                tok = token::create_new(TK_PLUSEQ, "+=", line_num);
            } else {
                tok = token::create_new(TK_PLUS, std::string(1, current_char), line_num);
            }
            break;
        
        case '-':
            if (peek_char() == '=') {
                read_char();
                tok = token::create_new(TK_MINUSEQ, "-=", line_num);
            } else {
                tok = token::create_new(TK_MINUS, std::string(1, current_char), line_num);
            }
            break;

        
        case '*':
            if (peek_char() == '=') {
                read_char();
                tok = token::create_new(TK_TIMESEQ, "*=", line_num);
            } else {
                tok = token::create_new(TK_ASTERISK, std::string(1, current_char), line_num);
            }
            break;
        
        case '%':
            if (peek_char() == '=') {
                read_char();
                tok = token::create_new(TK_MODEQ, "%=", line_num);
            } else {
                tok = token::create_new(TK_MOD, std::string(1, current_char), line_num);
            }
            break;
        
        case '<':
            if (peek_char() == '=') {
                read_char();
                tok = token::create_new(TK_LTEQ, "<=", line_num);
            } else if (peek_char() == '<') {
                read_char();
                if (peek_char() == '=') {
                    read_char();
                    tok = token::create_new(token_kind::TK_LSHIFTEQ, "<<=", line_num);
                } else {
                    tok = token::create_new(TK_LTEQ, "<=", line_num);
                }
            } else {
                tok = token::create_new(TK_LT, std::string(1, current_char), line_num);
            }
            break;
        
        case '>':
            if (peek_char() == '=') {
                read_char();
                tok = token::create_new(TK_GTEQ, ">=", line_num);
            } else if (peek_char() == '>') {
                read_char();
                if (peek_char() == '=') {
                    read_char();
                    tok = token::create_new(token_kind::TK_RSHIFTEQ, ">>=", line_num);
                } else {
                    tok = token::create_new(TK_GTEQ, ">=", line_num);
                }
            } else {
                tok = token::create_new(TK_GT, std::string(1, current_char), line_num);
            }
            break;

        case '!':
            if (peek_char() == '=') {
                read_char();
                tok = token::create_new(token_kind::TK_NEQUALTO, "!=", line_num);
            } else {
                tok = token::create_new(TK_BANG, std::string(1, current_char), line_num);
            }
            break;

        case '&':
            if (peek_char() == '=') {
                read_char();
                tok = token::create_new(token_kind::TK_ANDEQ, "&=", line_num);
            } else if (peek_char() == '&') {
                read_char();
                tok = token::create_new(token_kind::TK_LOG_AND, "&&", line_num);
            } else {
                tok = token::create_new(token_kind::TK_AMPERSAND, std::string(1, current_char), line_num);
            }
            break;
        
        case '|':
            if (peek_char() == '=') {
                read_char();
                tok = token::create_new(token_kind::TK_OREQ, "|=", line_num);
            } else if (peek_char() == '|') {
                read_char();
                tok = token::create_new(token_kind::TK_LOG_OR, "||", line_num);
            } else {
                tok = token::create_new(token_kind::TK_PIPE, std::string(1, current_char), line_num);
            }
            break;
        
        case '^':
            if (peek_char() == '=') {
                read_char();
                tok = token::create_new(token_kind::TK_XOREQ, "^=", line_num);
            } else {
                tok = token::create_new(token_kind::TK_CARET, std::string(1, current_char), line_num);
            }
            break;

        case '~':
            if (peek_char() == '=') {
                read_char();
                tok = token::create_new(token_kind::TK_NEGATEEQ, "~=", line_num);
            } else {
                tok = token::create_new(token_kind::TK_TILDE, std::string(1, current_char), line_num);
            }
            break;

        case ':':
            if (peek_char() == ':') {
                read_char();
                tok = token::create_new(token_kind::TK_DOUBLECOLON, "::", line_num);
            } else {
                tok = token::create_new(token_kind::TK_COLON, std::string(1, current_char), line_num);
            }
            break;

        case '\0':
            tok = token::create_new(token_kind::TK_EOF, "__%internal_eof", line_num);
            break;
            
        default:
            if (isalpha(current_char) != 0) {
                tok.name = read_identifier();
                tok.kind = lookup_identifier(tok.name);
                tok.line_num = line_num;
                tokens.push_back(tok);
                return tok;
            } else if (isdigit(current_char) != 0) {
                tok = read_number();
                tok.line_num = line_num;
                tokens.push_back(tok);
                return tok;
            } else {
                tok = token::create_new(token_kind::TK_ILLEGAL, "__%internal_illegal", line_num);
                std::printf("Illegal token '%s' on line %ul in file %s", 
                    tok.name.c_str(),
                    line_num,
                    file_ptr->name.c_str()
                );
            }
            break;
    }

    read_char();
    tokens.push_back(tok);
    return tok;
}


/// @brief Tokenize the input
static void tokenize() {
    token tok;
    read_char();
    tok.kind = token_kind::TK_ILLEGAL;
    while (tok.kind != token_kind::TK_EOF) {
        tok = next_token();
    }
}


/// @brief Entrypoint for the tokenizer
std::vector<token> tokenize_file(viper::VFile* file) {
    file_ptr = file;
    line_num = 0;
    position = 0;
    read_position = 0;
    input_ptr = &file->content;
    canonicalize_newline(*input_ptr);

    keywords["const"] = TK_CONST;
    keywords["let"] = TK_LET;
    keywords["proc"] = TK_PROC;
    keywords["return"] = TK_RETURN;
    keywords["struct"] = TK_STRUCT;
    keywords["enum"] = TK_ENUM;
    keywords["module"] = TK_MODULE;
    keywords["void"] = TK_VOID;
    keywords["for"] = TK_FOR;
    keywords["while"] = TK_WHILE;
    keywords["if"] = TK_IF;
    keywords["elif"] = TK_ELIF;
    keywords["else"] = TK_ELSE;

    keywords["true"] = TK_TRUE;
    keywords["false"] = TK_FALSE;

    keywords["i8"] = TK_TYPESPEC_I8;   
    keywords["i16"] = TK_TYPESPEC_I16;  
    keywords["i32"] = TK_TYPESPEC_I32;  
    keywords["i64"] = TK_TYPESPEC_I64;  
    keywords["f32"] = TK_TYPESPEC_F32;  
    keywords["f64"] = TK_TYPESPEC_F64;  
    keywords["u8"] = TK_TYPESPEC_U8;   
    keywords["u16"] = TK_TYPESPEC_U16;  
    keywords["u32"] = TK_TYPESPEC_U32;  
    keywords["u64"] = TK_TYPESPEC_U64;  
    keywords["bool"] = TK_TYPESPEC_BOOL; 

    tokenize();
    return tokens;
}

} // tokenizer namespace
} // viper namespace
