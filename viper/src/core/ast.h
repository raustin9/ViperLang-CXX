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

// Operator precedences
using prec_e = enum precedence {
    LOWEST = 0,
    INVALID_OP,
    ASSIGN,
    COMPARISON,
    LOGICAL_OR_AND,
    ADDSUB,
    MULDIVMOD,
    BITSHIFT,
    PREFIX,
    CALL,
};

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
    virtual void print() {}

private:

    Context context;
    ASTNode* parent_node;
    std::string module;
    Scope* scope;
};

//////////////////////////////////////
///          EXPRESSIONS           ///
//////////////////////////////////////

/* Represents an expression. Evaluates to a value */
struct ExpressionNode : public ASTNode {
};

/* Represents an expression statement. 
 * x = 10 + b;
 * x + 2;
 */
struct ExpressionStatementNode : public ASTNode {
    ExpressionNode* expr;

    void print() override {
        expr->print();
    }
};

/* Represents an expression with a prefix operator
 * !x
 * ~x
 */
struct ExpressionPrefixNode : public ExpressionNode {
    token op;
    ExpressionNode* rhs;

    void print() override {
        std::printf("%s", op.name.c_str());
        rhs->print();
    }
};

/* Represents an expression with an infix operator
 * x + y
 * y % 6
 */
struct ExpressionBinaryNode : public ExpressionNode {
    ExpressionNode* lhs;
    token op;
    ExpressionNode* rhs;

    void print() override {
        std::printf("[");
        lhs->print();
        std::printf(" %s ", op.name.c_str());
        rhs->print();
        std::printf("]");
    }
};

/* Represents a string literal expression
 * "example"
 */
struct ExpressionStringLiteralNode : public ExpressionNode {
    token tok;

    void print() override {
        std::printf("\"%s\"", tok.name.c_str());
    }
};

/* Represents a procedure call
 * foo()
 * bar(x + 1, num_seconds)
 */
struct ExpressionProcedureCallNode : public ExpressionNode {
    token identifier;
    std::vector<ExpressionNode*> arguments;
    void print() override {
        std::printf("%s(", identifier.name.c_str());
        for (const auto& arg : arguments) {
            arg->print();
            std::printf(", ");
        }
        std::printf(")");
    }
};

/* Represents an identifier reference
 * num_seconds
 * example_array[...]
 */
struct ExpressionIdentifierNode : public ExpressionNode {
    token identifier;
    ExpressionNode* expr; // for dimensional access

    void print() override {
        std::printf("%s", identifier.name.c_str());
        if (expr != nullptr) {
            std::printf("[");
            expr->print();
            std::printf("]");
        }
    }
};


/*
 * Represents a member access expression for a struct
 * test_struct.field;
 * test_struct.method();
 */
struct ExpressionMemberAccessNode : public ExpressionNode {
    token identifier;
    ExpressionNode* access;

    void print() override {
        std::printf("%s.", identifier.name.c_str());
        access->print();
    }
};


/* Represents an integer literal */
struct IntegerLiteralNode : public ExpressionNode {
    IntegerLiteralNode(u64 value) : value(value) {}

    void print() override {
        std::printf("%lu", this->value);
    }

    u64 value;
};

/* Represents a boolean "true" or "false" literal expression */
struct BooleanLiteralNode : public ExpressionNode {
    BooleanLiteralNode(bool is_true) : is_true(is_true) {}

    void print() override {
        if (is_true) {
            std::printf("true");
        } else {
            std::printf("false");
        }
    }

    bool is_true; // whether or not this evalueates to true or false
};

/* Represents a floating point number literal */
struct FloatLiteralNode : public ExpressionNode {
    FloatLiteralNode(f64 value) : value(value) {}

    void print() override {
        std::printf("%lf", value);
    }

    f64 value;
};

/////////////////////////////////////
///          STATEMENTS           ///
/////////////////////////////////////

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

    void print() override {
        // Print function prototype
        std::printf("proc <%s>: <%s> (", name.c_str(), return_declarator->tok.name.c_str());
        for (const auto& param : this->parameters) {
            param->print();
            std::printf(", ");
        }
        std::printf(") {\n");

        /// Print the code body
        for (const auto& stmt : code_body) {
            std::printf("    ");
            stmt->print();
            std::printf("\n");
        }

        std::printf("}\n");
    }

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

    void print() override {
        std::printf("<%s>: <%s>", this->name.c_str(), data_type.name.c_str());
    }

    private:
    std::string name;
    token data_type;
};

/* Represents the declaration of a new variable 
   let x: i32 = 0;
 * */
struct VariableDeclarationNode : public ASTNode {
    VariableDeclarationNode(
        const std::string& name,
        ASTNode* type_spec,
        ASTNode* expr
    ) : name(name), name_mangled(name), type_spec(type_spec), value(expr) {}
    ~VariableDeclarationNode() {}

    std::string name;
    std::string name_mangled;
    ASTNode* type_spec;
    ASTNode* value;

    void print() override {
        std::printf("let <%s>: <%s> = ", name.c_str(), type_spec->tok.name.c_str());
        this->value->print();
    }
};

/* Represents a return statement from a function
 * return x;
 */
struct ReturnStatementNode : public ASTNode {
    ExpressionNode* expr;

    void print() override {
        std::printf("return ");
        expr->print();
    }
};

/* Represents a conditional statement
 * if (condition) {
 *  ...
 * } elif (condition) {
 *  ...
 * } else {
 *  ...
 * }
 */
struct ConditionalStatementNode : public ASTNode {
    token tok;
    ExpressionNode* condition;  // condition to evaluate 
    std::vector<ASTNode*> body; // code body

    ASTNode* else_claus;        // else of elif statement
    
    void print() {
        std::printf("%s ", tok.name.c_str());
        if (condition != nullptr) {
            condition->print();
        }
        std::printf(" {\n");
        for (const auto& stmt : body) {
            std::printf("    ");
            stmt->print();
        }
        std::printf("\n}");

        if (else_claus != nullptr) {
            else_claus->print();
        }
        std::printf("\n");
    }
};

/* Represents the definition of a struct
 * struct Test {
 *     member :: type;
 * }
 */
struct StructDefinitionNode : public ASTNode {
    token identifier;
    std::vector<ASTNode*> fields;

    void print() override {
        std::printf("struct %s {\n", identifier.name.c_str());
        for (const auto& field : fields) {
            std::printf("    ");
            field->print();
            std::printf("\n");
        }
        std::printf("}\n");
    }
};

/* Represents a field member within a struct definition
 * struct Test {
 *     member :: type;
 * }
 */
struct StructMemberFieldNode : public ASTNode {
    token identifier;
    ASTNode* type_spec;

    void print() override {
        std::printf("%s :: %s", identifier.name.c_str(), type_spec->tok.name.c_str());
    }
};

/* The structure for the 
 * abstract syntax tree */
struct AST {
    ASTNode* head;

    static std::shared_ptr<AST> create_new();
    const std::vector<ASTNode*>& get_nodes() const;
    
    void add_node(ASTNode* node);

    void print_tree() {
        for (const auto& node : nodes) {
            node->print();
            std::printf("\n");
        }
    }

    private:
        AST() {}
        std::vector<ASTNode*> nodes;
};

}
