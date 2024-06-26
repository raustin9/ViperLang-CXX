#include "tokenizer.h"
#include "token.h"
#include "platform/platform.h"
#include <cctype>
#include <unordered_map>
#include <iostream>

namespace viper {

/// TODO: This is bugged, but it is probably good for now
/// @brief Turn all \r or \r\n into just \n
void Tokenizer::canonicalize_newline() {
    u64 i = 0, j = 0;

    while (
        m_input_ptr->at(i) && 
        i < m_input_ptr->size()-1
    ) {
        if (m_input_ptr->at(i) == '\r' && m_input_ptr->at(i+1) == '\n') {
            i += 2;
            m_input_ptr->at(j++) = '\n';
        } else if (m_input_ptr->at(i) == '\r') {
            i++;
            m_input_ptr->at(j++) = '\n';
        } else {
            m_input_ptr->at(j++) = m_input_ptr->at(i++);
        }
    }
    m_input_ptr->at(j) = '\0';
}

/// @brief Eat a character and put it in current_char
void Tokenizer::read_char() {
    if(read_position >= m_input_ptr->length()) {
        current_char = '\0';
    } else {
        current_char = m_input_ptr->at(read_position);
    }

    position = read_position;
    read_position++;
}

/// @brief Skip over whitespace characters
void Tokenizer::skip_whitespace() {
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
token Tokenizer::read_number() {
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

    tok.name = m_input_ptr->substr(pos, position-pos);
    return tok;
}


/// @brief Look at the next character in input without incrimenting
char Tokenizer::peek_char() {
    if (read_position >= m_input_ptr->length()) {
        return '\0';
    }
    
    return m_input_ptr->at(read_position);
}


/// @brief Lookup an identifier string in the table of keywords, and if 
///        it is not a keyword it is just normal identifier
token_kind Tokenizer::lookup_identifier(const std::string& identifier) {
    if (keywords.find(identifier) != keywords.end()) {
        return keywords[identifier];
    }

    return token_kind::TK_IDENT;
}


/// @brief Read an identifier string
std::string Tokenizer::read_identifier() {
    u32 pos = position;
    while (isalpha(current_char) != 0 ||
           current_char == '_' ||
           isdigit(current_char) != 0) {
        read_char();
    }

    return m_input_ptr->substr(pos, position-pos);
}


/// @brief Skip over multi line comments
token Tokenizer::skip_single_line_comment() {
    while (current_char != '\n') {
        read_char();
    }

    return token::create_new(TK_COMMENT, "__%internal_comment_single_line__", 0);
}


/// @brief Skip block comment
token Tokenizer::skip_multi_line_comment() {
    if (m_input_ptr->find("*/") == std::string::npos) {
        std::cout << "Unclosed block comment";
        return token::create_new(TK_ILLEGAL, "__%internal_illegal__", 0);
    }
    while (1) {
        if (current_char == '*' && peek_char() == '/') {
            read_char();
            break;
        }
    }
    
    return token::create_new(TK_COMMENT, "__%internal_comment_multi_line__", 0);
}


/// @brief Read the content of a string literal
std::string Tokenizer::read_string_content() {
    read_char();
    u32 pos = position;
    while (current_char != '"') {
        read_char(); 
    }

    return m_input_ptr->substr(pos, position-pos);
}


/// @brief Get the next token and return it
token Tokenizer::next_token() {
    token tok;

loop_begin:
    skip_whitespace();
    switch(current_char) {
        case '/':
            if (peek_char() == '/') {
                (void)skip_single_line_comment();
                goto loop_begin;
            } else if (peek_char() == '*') {
                (void)skip_multi_line_comment();
                goto loop_begin;
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

        case '"':
            tok = token::create_new(TK_STR, read_string_content(), line_num);
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
                platform::print_error("Illegal token '%s' on line %lu in file %s",
                    tok.name.c_str(),
                    line_num,
                    m_file->name.c_str()
                );
//                std::printf("Illegal token '%s' on line %lu in file %s", 
//                    tok.name.c_str(),
//                    line_num,
//                    m_file->name.c_str()
//                );
            }
            break;
    }

    read_char();
    tokens.push_back(tok);
    return tok;
}


/// @brief Tokenize the input
void Tokenizer::tokenize() {
    token tok;
    // read_char();
    tok.kind = token_kind::TK_ILLEGAL;
    while (tok.kind != token_kind::TK_EOF) {
        tok = next_token();
    }
}

Tokenizer Tokenizer::create_new(VFile* file) {
    Tokenizer tok = Tokenizer();
    tok.m_file = file;
    tok.position = 0;
    tok.read_position = 0;
    tok.line_num = 0;
    tok.m_input_ptr = &tok.m_file->content;
    // canonicalize_newline();

    tok.keywords["const"] = TK_CONST;
    tok.keywords["let"] = TK_LET;
    tok.keywords["define"] = TK_DEFINE;
    tok.keywords["return"] = TK_RETURN;
    tok.keywords["struct"] = TK_STRUCT;
    tok.keywords["enum"] = TK_ENUM;
    tok.keywords["module"] = TK_MODULE;
    tok.keywords["void"] = TK_VOID;
    tok.keywords["for"] = TK_FOR;
    tok.keywords["while"] = TK_WHILE;
    tok.keywords["do"] = TK_DO;
    tok.keywords["if"] = TK_IF;
    tok.keywords["elif"] = TK_ELIF;
    tok.keywords["else"] = TK_ELSE;
    
    tok.keywords["#define"] = TK_PP_DEFINE;
    tok.keywords["#import"] = TK_PP_IMPORT;
    tok.keywords["#export"] = TK_PP_EXPORT;

    tok.keywords["true"] = TK_TRUE;
    tok.keywords["false"] = TK_FALSE;

//    tok.keywords["byte"] = TK_TYPESPEC_BYTE;   
//    tok.keywords["i8"] = TK_TYPESPEC_I8;   
//    tok.keywords["i16"] = TK_TYPESPEC_I16;  
//    tok.keywords["i32"] = TK_TYPESPEC_I32;  
//    tok.keywords["i64"] = TK_TYPESPEC_I64;  
//    tok.keywords["f32"] = TK_TYPESPEC_F32;  
//    tok.keywords["f64"] = TK_TYPESPEC_F64;  
//    tok.keywords["u8"] = TK_TYPESPEC_U8;   
//    tok.keywords["u16"] = TK_TYPESPEC_U16;  
//    tok.keywords["u32"] = TK_TYPESPEC_U32;  
//    tok.keywords["u64"] = TK_TYPESPEC_U64;  
//    tok.keywords["bool"] = TK_TYPESPEC_BOOL; 
    tok.read_char();

    return tok;
}

/// @brief Entrypoint for the tokenizer
std::vector<token> Tokenizer::tokenize_file() {
    tokenize();
    return tokens;
}

} // viper namespace
