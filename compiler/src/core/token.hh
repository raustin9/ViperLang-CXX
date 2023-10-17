#pragma once
#include "defines.hh"
#include <string>

/*
 * Tokens
 * The Lexer (or Tokenize) takes in the transformed code from the preprocessor and 
 * outputs a Token Stream. A token is a representation of a keyword, operator, identifier,
 * etc from the code, and that is what is passed to the parser.
 *
 */

typedef enum Token {
    // Declaritive keywords
    TOK_PROC,       // "proc"
    TOK_INTERFACE,  // "interface"
    TOK_MUT,        // "mut"
    TOK_CONST,      // "const"
    TOK_IDENT,      // for identifiers like procedure, interface, and variable names
    TOK_IMPL,       // "impl" used for declaring block of code for interface implementation
    TOK_DECL,       // "::"

    // Data types
    TOK_VOID,   // "void"
    TOK_INT,    // "int"
    TOK_FLOAT,  // "float"
    TOK_BOOL,   // "bool"
    TOK_BYTE,   // "byte"
                
    TOK_NUMINT,   // integer literal
    TOK_NUMFLOAT, // float literal
                
    // Boolean types
    TOK_TRUE,   // "true"
    TOK_FALSE,  // "false"

    // Operators
    TOK_ASSIGN,     // "=" used to assign value of an expression to something
    TOK_PLUS,       // "+" arithmetic addition
    TOK_MINUS,      // "-" arithmetic subtraction
    TOK_TIMES,      // "*" arithmetic multiplication
    TOK_DIV,        // "/" arithmetic division
    TOK_MOD,        // "%" arithmetic modulus
    TOK_LSHIFT,     // "<<" binary left shift
    TOK_RSHIFT,     // ">>" binary right shift
    TOK_PLUSEQ,     // "+=" adds then assigns
    TOK_MINUSEQ,    // "-="
    TOK_TIMESEQ,    // "*="
    TOK_DIVEQ,      // "/="
    TOK_MODEQ,      // "%="
    TOK_LSHIFTEQ,   // "<<="
    TOK_RSHIFTEQ,   // ">>="

    TOK_DOT,        // "." used to call internal functions of an interface
    TOK_COMMA,      // "," used to separate identifiers 

    TOK_BINARY_NOT, // "~" performs binary not
    TOK_BINARY_OR,  // "|" performs binary or
    TOK_BINARY_AND, // "&" performs binary and
    TOK_BINARY_XOR, // "^" performs binary xor

    TOK_INCR,   // "++" incriments the value by one
    TOK_DECR,   // "--" decrements the value by one

    // Comparison Operators
    TOK_EQUALTO,    // "==" compares two expressions to see if they are the same
    TOK_NOTEQUALTO, // "!=" compares two expressions to see if they are different
    TOK_LT,         // "<" less than comparison
    TOK_GT,         // ">" greater than comparison
    TOK_LTEQ,       // "<=" less than or equal to
    TOK_GTEQ,       // ">=" greater than or equal to

    // Boolean operators
    TOK_NOT,        // "!"
    TOK_COMP_AND,   // "&&"
    TOK_COMP_OR,    // "||"

    // Syntax Features
    TOK_RSQUIRLY,   // "}"
    TOK_LSQUIRLY,   // "{"
    TOK_RPAREN,     // ")"
    TOK_LPAREN,     // "("
    TOK_RSQUARE,    // "]"
    TOK_LSQUARE,    // "["
    TOK_SEMICOLON,  // ";"
 
    TOK_INVALID,    // for invalid reads
    TOK_EOF,        // end of file
    TOK_MAX_TOK
} token_e;

typedef struct TokenValue {
    token_e tok;
    std::string literal;
    size_t line_num;
} token_t;

token_t create_token(std::string literal, token_e token, size_t line_num);

void print_token(token_t tok);
