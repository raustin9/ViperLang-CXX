#pragma once

/*
 *  ast.h
 *
 *  This contains the interface for the Abstract Syntax Tree of the Viper language
 *
 */

#include "defines.h"
#include "token.h"

#include <memory>

namespace viper {

/* The kind of node in the AST */
enum NodeKind {
    AST_NOOP,

};

/* The data type of node in AST */
struct Type {
    enum TypeKind {
        BOOL,
        BYTE,
        I8,
        I16,
        I32,
        I64,
        U8,
        U16,
        U32,
        U64,
        F32,
        F64,
        ENUM,
        POINTER,
        PROCEDURE,
        ARRAY,
        STRUCTURE,
        UNION,
    };

};

/* Node in the AST */
struct ASTNode {
    NodeKind kind;
    token tok;

    static std::shared_ptr<ASTNode> create_new(token tok, NodeKind kind);
};

/* The structure for the 
 * abstract syntax tree */
struct AST {
    ASTNode* head;

    static std::shared_ptr<AST> create_new();

    private:
        AST() {}
};

}
