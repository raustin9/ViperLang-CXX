#pragma once

/*
 *  ast.h
 *
 *  This contains the interface for the Abstract Syntax Tree of the Viper language
 *
 */

#include "core/result.h"
#include "core/scope.h"
#include "defines.h"
#include "token.h"
#include "core/context.h"

#include <memory>

namespace viper {

/* The kind of node in the AST */
enum NodeKind {
    AST_NOOP,
    AST_INVALID_NODE
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
    ASTNode(NodeKind _kind) : kind(_kind) {}
    ASTNode() {}
    NodeKind kind;
    token tok;
    ASTNode* parent;

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
    ~ProcedureNode();
    
    std::string& get_name();
    void set_name(const std::string& name);

    void set_return_type(ASTNode* node);
    const ASTNode* get_return_type() const;

    void add_parameter(ASTNode* param);

    void add_statement(ASTNode* stmt);


    private:
    std::string name;                 // unmangled name of the procedure
    std::string lookup_name;        
    std::string mangled_name;         // the mangled name of the procedure
    std::vector<ASTNode*> parameters; // the parameter definitions for the function
    ASTNode* return_declarator;       
    ASTNode* procedure_declarator;    
    Scope* scope;
    std::vector<ASTNode*> code_body;  // code block body of the procedure
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

/* Represents the declaration of a new variable 
   let x: i32 = 0;
 * */
struct VariableDeclaration : public ASTNode {
    VariableDeclaration(
        const std::string& name,
        ASTNode* type_spec,
        ASTNode* expr
    ) : name(name), name_mangled(name), type_spec(type_spec), value(expr) {}
    ~VariableDeclaration() {}

    std::string name;
    std::string name_mangled;
    ASTNode* type_spec;
    ASTNode* value;
};

/* Represents an expression. Evaluates to a value */
struct Expression : public ASTNode {

};

/* Represents an integer literal */
struct IntegerLiteralNode : public Expression {
    IntegerLiteralNode(u64 value) : value(value) {}

    u64 value;
};

/* Represents a boolean "true" or "false" literal expression */
struct BooleanLiteralNode : public Expression {
    BooleanLiteralNode(bool is_true) : is_true(is_true) {}

    bool is_true; // whether or not this evalueates to true or false
};

/* Represents a floating point number literal */
struct FloatLiteralNode : public Expression {
    FloatLiteralNode(f64 value) : value(value) {}

    f64 value;
};

/* The structure for the 
 * abstract syntax tree */
struct AST {
    ASTNode* head;

    static std::shared_ptr<AST> create_new();
    const std::vector<ASTNode*>& get_nodes() const;
    
    void add_node(ASTNode* node);

    private:
        AST() {}
        std::vector<ASTNode*> nodes;
};

}
