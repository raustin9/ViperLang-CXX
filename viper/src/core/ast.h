#pragma once

/*
 *  ast.h
 *
 *  This contains the interface for the Abstract Syntax Tree of the Viper language
 *
 */

#include "core/scope.h"
#include "defines.h"
#include "token.h"
#include "core/context.h"

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

    Context& getContext() {return context;}
    void setContext(Context context) {}

private:

    Context context;
    ASTNode* parent_node;
    std::string module;
    Scope* scope;
};

// For procedure nodes
struct ProcedureNode : public ASTNode {
    ProcedureNode();
    
    std::string& get_name();
    void set_name(const std::string& name);
    void add_parameter(ASTNode* param) {
        parameters.push_back(param);
    }

    private:
    std::string name;
    std::string lookup_name;
    std::string mangled_name;
    std::vector<ASTNode*> parameters;
    ASTNode* return_declarator;
    ASTNode* procedure_declarator;

    const Type* data_type;

};

struct ProcParameter : public ASTNode {
    void set_name(const std::string& str) {
        name = str;
    }
    std::string get_name() const { 
        return name;
    }

    void set_data_type(const token& tok) {
        data_type = tok;
    }
    token get_data_type() const {
        return data_type;
    }

    private:
    std::string name;
    token data_type;
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
