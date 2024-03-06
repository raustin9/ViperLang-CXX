#pragma once

#include "defines.h"

#include <string>

namespace viper {

// Kinds of tokens that exist
enum token_kind {
    TK_ILLEGAL,
    TK_IDENT,
  
    // Keywords
    TK_CONST,
    TK_LET,
    TK_PROC,
    TK_RETURN,
    TK_STRUCT,
    TK_ENUM,
    TK_UNION,
    TK_CONTINUE,
    TK_BREAK,
    TK_SWITCH,
    TK_CASE,
    TK_DEFAULT,

    TK_MODULE,
    TK_VOID,
    TK_FOR,
    TK_WHILE,
    TK_IF,
    TK_ELIF,
    TK_ELSE,
   
    // Hold internal data types
    TK_NUM_FLOAT, // floating point numbers
    TK_NUM_INT,   // integer numbers
    TK_PP_NUM,
    TK_TRUE,
    TK_FALSE,

    // Type specifiers
    TK_TYPESPEC_I8,   // i8
    TK_TYPESPEC_I16,  // i16
    TK_TYPESPEC_I32,  // i32
    TK_TYPESPEC_I64,  // i64
    TK_TYPESPEC_F32,  // f32
    TK_TYPESPEC_F64,  // f64
    TK_TYPESPEC_U8,   // u8
    TK_TYPESPEC_U16,  // u16
    TK_TYPESPEC_U32,  // u32
    TK_TYPESPEC_U64,  // u64
    TK_TYPESPEC_BOOL, // bool 

    // Punctuators
    TK_AMPERSAND, // &
    TK_ASSIGN,    // =
    TK_EQUALTO,   // ==
    TK_NEQUALTO,  // !=
    TK_PLUS,      // +
    TK_MINUS,     // -
    TK_ASTERISK,  // *
    TK_SLASH,     // /
    TK_BANG,      // !
    TK_TILDE,     // ~
    TK_MOD,       // %
    TK_LSHIFT,    // <<
    TK_RSHIFT,    // >>
    TK_LT,        // <
    TK_GT,        // >
    TK_GTEQ,      // >=
    TK_LTEQ,      // <=
    TK_TIMESEQ,   // *=
    TK_DIVEQ,     // /=
    TK_PLUSEQ,    // +=
    TK_MINUSEQ,   // -=
    TK_MODEQ,     // %=
    TK_LSHIFTEQ,  // <<=
    TK_RSHIFTEQ,  // >>=
    TK_ANDEQ,     // &=
    TK_OREQ,      // |=
    TK_XOREQ,     // ^=
    TK_NEGATEEQ,  // ~=

    TK_PIPE,  // |
    TK_CARET, // ^
    
    TK_LOG_AND, // &&
    TK_LOG_OR,  // ||

    TK_COMMA,        // ,
    TK_DOT,          // .
    TK_COLON,        // :
    TK_SEMICOLON,    // ;
    TK_DOUBLECOLON,  // ::
    
    TK_LPAREN,   // (
    TK_RPAREN,   // )
    TK_LBRACKET, // [
    TK_RBRACKET, // ]
    TK_LSQUIRLY, // {
    TK_RSQUIRLY, // }

    TK_EOF,
};

// Source codes files
struct VFile {
    std::string name;
    i32 file_number;
    std::string content;

    // From chibic project. Maybe use?
    std::string display_name;
    i32 line_delta;
};

// Data type for a token
struct token {
    token_kind kind;
    i64 value;
    f64 fvalue;
    std::string name;
    
    VFile* file;
    u32 line_num;

    static token create_new(token_kind kind, std::string name, u32 line_num) {
        std::printf("NAME: |%s|\n", name.c_str());
        token tok;
        tok.kind = kind;
        tok.name = name;
        tok.line_num = line_num;
        return tok;
    }

    void print_name() const {
        std::printf("Name: %s.\n", name.c_str());
    }
};

}
