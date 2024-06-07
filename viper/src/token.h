#pragma once

#include "defines.h"
#include "core/span.h"
#include "core/core.h"

#include <string>
#include <unordered_map>
#include <utility>

namespace viper {

// Kinds of tokens that exist
enum token_kind {
    TK_ILLEGAL,
    TK_IDENT,
  
    // Keywords
    TK_CONST,
    TK_LET,
    TK_DEFINE,
    TK_RETURN,
    TK_STRUCT,
    TK_ENUM,
    TK_MATCH,
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
    TK_DO,
    TK_IF,
    TK_ELIF,
    TK_ELSE,

    // Preprocessor keywords
    TK_PP_DEFINE,
    TK_PP_IMPORT,
    TK_PP_EXPORT,

    TK_STR,
   
    // Hold internal data types
    TK_NUM_FLOAT, // floating point numbers
    TK_NUM_INT,   // integer numbers
    TK_PP_NUM,
    TK_TRUE,
    TK_FALSE,

    // Type specifiers
    TK_TYPESPEC_BYTE, // 'byte' -- equivalent to u8
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

    TK_COMMENT,  // represents a comment. Meant to be skipped
    TK_EOF,
};

// Data type for a token
struct token {
    token() {}

    token(const token& other) {
        kind = other.kind;
        value = other.value;
        fvalue = other.fvalue;
        name = other.name;
        file = other.file;
        line_num = other.line_num;
    }
    token(token&& other) {
        std::swap(other.kind, kind);
        std::swap(other.value, value);
        std::swap(other.fvalue, fvalue);
        std::swap(other.name, name);
        std::swap(other.line_num, line_num);
        std::swap(other.file, file);
        std::swap(other.kind, kind);
    }

    token& operator=(token other) {
        kind = other.kind;
        value = other.value;
        fvalue = other.fvalue;
        name = other.name;
        file = other.file;
        line_num = other.line_num;

        return *this;
    }

    token_kind kind;
    u64 value;
    f64 fvalue;
    std::string name;
    //Span span;
    
    viper::VFile* file;
    u32 line_num;

    static token create_new(token_kind kind, std::string name, u64 line_num) {
        token tok;
        tok.kind = kind;
        tok.name = name;
        tok.line_num = line_num;
        return tok;
    }

    static std::string kind_to_str(token_kind kind) {
        std::unordered_map<token_kind, std::string> kind_map;
        kind_map[TK_ILLEGAL] = "TK_ILLEGAL";
        kind_map[TK_IDENT] = "TK_IDENT";
        kind_map[TK_CONST] = "TK_CONST";
        kind_map[TK_LET] = "TK_LET";
        kind_map[TK_DEFINE] = "TK_DEFINE";
        kind_map[TK_RETURN] = "TK_RETURN";
        kind_map[TK_STRUCT] = "TK_STRUCT";
        kind_map[TK_ENUM] = "TK_ENUM";
        kind_map[TK_UNION] = "TK_UNION";
        kind_map[TK_CONTINUE] = "TK_CONTINUE";
        kind_map[TK_BREAK] = "TK_BREAK";
        kind_map[TK_SWITCH] = "TK_SWITCH";
        kind_map[TK_CASE] = "TK_CASE";
        kind_map[TK_DEFAULT] = "TK_DEFAULT";
        kind_map[TK_MODULE] = "TK_MODULE";
        kind_map[TK_VOID] = "TK_VOID";
        kind_map[TK_FOR] = "TK_FOR";
        kind_map[TK_WHILE] = "TK_WHILE";
        kind_map[TK_IF] = "TK_IF";
        kind_map[TK_ELIF] = "TK_ELIF";
        kind_map[TK_ELSE] = "TK_ELSE";
        kind_map[TK_NUM_FLOAT] = "TK_NUM_FLOAT" ;
        kind_map[TK_NUM_INT] =   "TK_NUM_INT" ;
        kind_map[TK_PP_NUM] = "TK_PP_NUM";
        kind_map[TK_TRUE] = "TK_TRUE";
        kind_map[TK_FALSE] = "TK_FALSE";
        kind_map[TK_TYPESPEC_I8] =   "TK_TYPESPEC_I8" ;
        kind_map[TK_TYPESPEC_I16] =  "TK_TYPESPEC_I16" ;
        kind_map[TK_TYPESPEC_I32] =  "TK_TYPESPEC_I32" ;
        kind_map[TK_TYPESPEC_I64] =  "TK_TYPESPEC_I64" ;
        kind_map[TK_TYPESPEC_F32] =  "TK_TYPESPEC_F32" ;
        kind_map[TK_TYPESPEC_F64] =  "TK_TYPESPEC_F64" ;
        kind_map[TK_TYPESPEC_U8] =   "TK_TYPESPEC_U8" ;
        kind_map[TK_TYPESPEC_U16] =  "TK_TYPESPEC_U16" ;
        kind_map[TK_TYPESPEC_U32] =  "TK_TYPESPEC_U32" ;
        kind_map[TK_TYPESPEC_U64] =  "TK_TYPESPEC_U64" ;
        kind_map[TK_TYPESPEC_BOOL] = "TK_TYPESPEC_BOOL" ;
        kind_map[TK_AMPERSAND] = "TK_AMPERSAND" ;
        kind_map[TK_ASSIGN] =    "TK_ASSIGN" ;
        kind_map[TK_EQUALTO] =   "TK_EQUALTO" ;
        kind_map[TK_NEQUALTO] =  "TK_NEQUALTO" ;
        kind_map[TK_PLUS] =      "TK_PLUS" ;
        kind_map[TK_MINUS] =     "TK_MINUS" ;
        kind_map[TK_ASTERISK] =  "TK_ASTERISK" ;
        kind_map[TK_SLASH] =     "TK_SLASH" ;
        kind_map[TK_BANG] =      "TK_BANG" ;
        kind_map[TK_TILDE] =     "TK_TILDE" ;
        kind_map[TK_MOD] =       "TK_MOD" ;
        kind_map[TK_LSHIFT] =    "TK_LSHIFT" ;
        kind_map[TK_RSHIFT] =    "TK_RSHIFT" ;
        kind_map[TK_LT] =        "TK_LT" ;
        kind_map[TK_GT] =        "TK_GT" ;
        kind_map[TK_GTEQ] =      "TK_GTEQ" ;
        kind_map[TK_LTEQ] =      "TK_LTEQ" ;
        kind_map[TK_TIMESEQ] =   "TK_TIMESEQ" ;
        kind_map[TK_DIVEQ] =     "TK_DIVEQ" ;
        kind_map[TK_PLUSEQ] =    "TK_PLUSEQ" ;
        kind_map[TK_MINUSEQ] =   "TK_MINUSEQ" ;
        kind_map[TK_MODEQ] =     "TK_MODEQ" ;
        kind_map[TK_LSHIFTEQ] =  "TK_LSHIFTEQ" ;
        kind_map[TK_RSHIFTEQ] =  "TK_RSHIFTEQ" ;
        kind_map[TK_ANDEQ] =     "TK_ANDEQ" ;
        kind_map[TK_OREQ] =      "TK_OREQ" ;
        kind_map[TK_XOREQ] =     "TK_XOREQ" ;
        kind_map[TK_NEGATEEQ] =  "TK_NEGATEEQ" ;
        kind_map[TK_PIPE] =  "TK_PIPE" ;
        kind_map[TK_CARET] = "TK_CARET" ;
        kind_map[TK_LOG_AND] = "TK_LOG_AND" ;
        kind_map[TK_LOG_OR] =  "TK_LOG_OR" ;
        kind_map[TK_COMMA] =        "TK_COMMA" ;
        kind_map[TK_DOT] =          "TK_DOT" ;
        kind_map[TK_COLON] =        "TK_COLON" ;
        kind_map[TK_SEMICOLON] =    "TK_SEMICOLON" ;
        kind_map[TK_DOUBLECOLON] =  "TK_DOUBLECOLON" ;
        kind_map[TK_LPAREN] =   "TK_LPAREN" ;
        kind_map[TK_RPAREN] =   "TK_RPAREN" ;
        kind_map[TK_LBRACKET] = "TK_LBRACKET" ;
        kind_map[TK_RBRACKET] = "TK_RBRACKET" ;
        kind_map[TK_LSQUIRLY] = "TK_LSQUIRLY" ;
        kind_map[TK_RSQUIRLY] = "TK_RSQUIRLY";
        kind_map[TK_EOF] = "TK_EOF";

        return kind_map[kind];
    }

    void print_name() const {
        std::printf("Name: %s Kind: %s\n", name.c_str(), kind_to_str(kind).c_str());
    }
};

}
