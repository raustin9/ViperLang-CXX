#pragma once

/*
 *  ast.h
 *
 *  This contains the interface for the Abstract Syntax Tree of the Viper language
 *
 */

#include "core/result.h"
#include "core/type.h"
#include "core/scope.h"
#include "defines.h"
#include "token.h"
#include "core/context.h"
#include "core/result.h"

#include <memory>

namespace viper {

// Operator precedences
using prec_e = enum precedence {
    LOWEST = 0,
    INVALID_OP,
    ASSIGN,
    LOGICAL_OR_AND,
    COMPARISON,
    ADDSUB,
    MULDIVMOD,
    BITSHIFT,
    PREFIX,
    CALL,
};

enum NodeKind {
    AST_NOOP,
    AST_INVALID_NODE
};

using ResultNode = result::Result<ASTNode*, VError>;

/* Node in the AST */
struct ASTNode {
    ASTNode(NodeKind kind) : kind(kind) {}
    ASTNode() {}
   
    NodeKind kind;
    token tok;

    void set_parent(ASTNode* node) {
        parent_node = node;
    }

    static std::shared_ptr<ASTNode> create_new(token tok, NodeKind kind);

    Context& getContext() {return context;}
    void setContext(Context context) {}
    virtual void print(const std::string& prepend) {}
    virtual void add_symbols(Scope* scope) {}
    virtual void set_scope(Scope* scope) {}

protected:

    Context context;
    ASTNode* parent_node;
    std::string module;
    Scope* scope;
};

/* Represents a block of code consisting of 
 * a series of statements
 *
 * {
 *     let i: i32 = 0;
 *     foo();
 * }
 */
struct CodeBlockStatementNode : public ASTNode {
    // Add line of code to the body
    void add_stmt(ASTNode* stmt) {
        body.push_back(stmt);
    }
    const std::vector<ASTNode*>& get_body() const {
        return body;
    }

    void print(const std::string& prepend) override {
        std::printf("%s{\n", prepend.c_str());
        for (const auto& stmt : body) {
            stmt->print("    " + prepend);
            std::printf("\n");
        }
        std::printf("%s}", prepend.c_str());
    }

    virtual void add_symbols(Scope* scope) override {
        for (const auto& stmt : body) {
            stmt->add_symbols(this->scope);
        }
    }

    private:
    std::vector<ASTNode*> body;
};

//////////////////////////////////////
///          EXPRESSIONS           ///
//////////////////////////////////////

/* Represents an expression. Evaluates to a value */
struct ExpressionNode : public ASTNode {
    Type* type;
    virtual Type* get_type() {return nullptr;}
};

/* Represents an expression statement. 
 * x = 10 + b;
 * x + 2;
 */
struct ExpressionStatementNode : public ASTNode {
    void set_expr(ExpressionNode* node) {
        expr = node;
    }

    const ExpressionNode* get_expr() const {
        return expr;
    }

    void print(const std::string& prepend) override {
        expr->print("    ");
    }

    private:
    ExpressionNode* expr;
};

/* Represents an expression with a prefix operator
 * !x
 * ~x
 */
struct ExpressionPrefixNode : public ExpressionNode {
    void set_rhs(ExpressionNode* node) {
        rhs = node;
    }
    
    void set_rhs(ResultNode& expr_node) {
        rhs = 
            static_cast<ExpressionNode*>(expr_node.unwrap_or(new ExpressionNode()));
    }

    const ExpressionNode* get_rhs() const {
        return rhs;
    }

    void set_operator(const token& optr) {
        op = optr;
    }

    const token& get_operator() const {
        return op;
    }

    void print(const std::string& prepend) override {
        std::printf("%s", op.name.c_str());
        rhs->print("    ");
    }

    Type* get_type() override {
        
        return nullptr;
    }

    private:
    token op;
    ExpressionNode* rhs;
};

/* Represents an expression with an infix operator
 * x + y
 * y % 6
 */
struct ExpressionBinaryNode : public ExpressionNode {
    void print(const std::string& prepend) override {
        std::printf("[");
        lhs->print("    ");
        std::printf(" %s ", op.name.c_str());
        rhs->print("    ");
        std::printf("]");
    }
    
    // Set the RHS of the expression
    void set_rhs(ExpressionNode* node) {
        rhs = node;
    }
    void set_rhs(ResultNode& expr_node) {
        rhs = 
            static_cast<ExpressionNode*>(expr_node.unwrap_or(new ExpressionNode()));
    }
    const ExpressionNode* get_rhs() const {
        return rhs;
    }

    // Set the operator
    void set_operator(const token& optr) {
        op = optr;
    }
    const token& get_operator() const {
        return op;
    }
   
    // Set the LHS of the expression
    void set_lhs(ExpressionNode* node) {
        lhs = node;
    }
    void set_lhs(ResultNode& expr_node) {
        lhs = 
            static_cast<ExpressionNode*>(expr_node.unwrap_or(new ExpressionNode()));
    }
    const ExpressionNode* get_lhs() const {
        return lhs;
    }


    private:
    ExpressionNode* lhs;
    token op;
    ExpressionNode* rhs;
};

/* Represents a string literal expression
 * "example"
 */
struct ExpressionStringLiteralNode : public ExpressionNode {
    void print(const std::string& prepend) override {
        std::printf("\"%s\"", tok.name.c_str());
    }

    void set_token(const token& t) {
        tok = t;
    }
    const token& get_token() const {
        return tok;
    }

    private:
    token tok;
};

/* Represents a procedure call
 * foo()
 * bar(x + 1, num_seconds)
 */
struct ExpressionProcedureCallNode : public ExpressionNode {
    void print(const std::string& prepend) override {
        std::printf("%s(", identifier.name.c_str());
        for (const auto& arg : arguments) {
            arg->print("    ");
            std::printf(", ");
        }
        std::printf(")");
    }

    void set_identifier(const token& t) {
        identifier = t;
    }
    const token& get_identifier() const {
        return identifier;
    }

    void add_argument(ExpressionNode* arg) {
        arguments.push_back(arg);
    }

    void add_argument(ResultNode r_arg) {
        arguments.push_back(
            static_cast<ExpressionNode*>(r_arg.unwrap_or(new ExpressionNode()))
        );
    }

    const std::vector<ExpressionNode*>& get_arguments() const {
        return arguments;
    }

    private:
    token identifier;
    std::vector<ExpressionNode*> arguments;
};

/* Represents an identifier reference
 * num_seconds
 * example_array[...]
 */
struct ExpressionIdentifierNode : public ExpressionNode {
    void print(const std::string& prepend) override {
        std::printf("%s", identifier.name.c_str());
        if (expr != nullptr) {
            std::printf("[");
            expr->print("    ");
            std::printf("]");
        }
    }

    void set_identifier(const token& t) {
        identifier = t;
    }
    const token& get_identifier() const {
        return identifier;
    }

    void set_expr(ExpressionNode* node) {
        expr = node;
    }
    
    void set_expr(ResultNode& expr_node) {
        expr = 
            static_cast<ExpressionNode*>(expr_node.unwrap_or(new ExpressionNode()));
    }

    const ExpressionNode* get_expr() const {
        return expr;
    }

    private:
    token identifier;
    ExpressionNode* expr; // for dimensional access
};


/*
 * Represents a member access expression for a struct
 * test_struct.field;
 * test_struct.method();
 */
struct ExpressionMemberAccessNode : public ExpressionNode {
    void print(const std::string& prepend) override {
        std::printf("%s.", identifier.name.c_str());
        access->print("    ");
    }

    void set_identifier(const token& t) {
        identifier = t;
    }
    const token& get_identifier() const {
        return identifier;
    }

    void set_access(ExpressionNode* node) {
        access = node;
    }
    
    void set_access(ResultNode& access_node) {
        access = 
            static_cast<ExpressionNode*>(access_node.unwrap_or(new ExpressionNode()));
    }

    const ExpressionNode* get_access() const {
        return access;
    }

    private:
    token identifier;
    ExpressionNode* access;
};


/* Represents an integer literal */
struct IntegerLiteralNode : public ExpressionNode {
    IntegerLiteralNode(u64 value) : value(value) {}

    void print(const std::string& prepend) override {
        std::printf("%lu", this->value);
    }

    void set_value(u64 val) {
        value = val;
    }

    u64 get_value() const {
        return value;
    }

    private:
    u64 value;
};

/* Represents a boolean "true" or "false" literal expression */
struct BooleanLiteralNode : public ExpressionNode {
    BooleanLiteralNode(bool is_true) : is_true(is_true) {}

    void print(const std::string& prepend) override {
        if (is_true) {
            std::printf("true");
        } else {
            std::printf("false");
        }
    }

    void set_is_true(bool is) {
        is_true = is;
    }

    bool get_is_true() const {
        return is_true;
    }

    private:
    bool is_true; // whether or not this evalueates to true or false
};

/* Represents a floating point number literal */
struct FloatLiteralNode : public ExpressionNode {
    FloatLiteralNode(f64 value) : value(value) {}

    void print(const std::string& prepend) override {
        std::printf("%lf", value);
    }

    void set_value(f64 val) {
        value = val;
    }

    f64 get_value() const {
        return value;
    }

    private:
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
    void set_body(CodeBlockStatementNode* b) {
        body = b;
    }

    void print(const std::string& prepend) override {
        // Print function prototype
        std::printf("%sproc <%s>: <%s> (", prepend.c_str(), name.c_str(), return_declarator->tok.name.c_str());
        for (const auto& param : this->parameters) {
            param->print("");
            std::printf(", ");
        }
        std::printf(")\n");

        body->print(prepend);
    }

    private:
    std::string name;                 // unmangled name of the procedure
    std::string lookup_name;        
    std::string mangled_name;         // the mangled name of the procedure
    std::vector<ASTNode*> parameters; // the parameter definitions for the function
    ASTNode* return_declarator;       
    ASTNode* procedure_declarator;    
    Scope* scope;
    CodeBlockStatementNode* body;
    // std::vector<ASTNode*> code_body;  // code block body of the procedure
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

    void print(const std::string& prepend) override {
        std::printf("%s<%s>: <%s>", prepend.c_str(), this->name.c_str(), data_type.name.c_str());
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

    void print(const std::string& prepend) override {
        std::printf("%slet <%s>: <%s> = ", prepend.c_str(), name.c_str(), type_spec->tok.name.c_str());
        this->value->print("    ");
    }

    void set_name(const std::string& str) {
        name = str;
    }
    const std::string& get_name() const {
        return name;
    }

    void set_name_mangled(const std::string& str) {
        name_mangled = str;
    }
    const std::string& get_name_mangled() const {
        return name_mangled;
    }

    void set_type_spec(ASTNode* ts) {
        type_spec = ts;
    }
    const ASTNode* get_type_spec() const {
        return type_spec;
    }

    void set_value(ASTNode* ts) {
        value = ts;
    }
    const ASTNode* get_value() const {
        return value;
    }

    private:
    std::string name;
    std::string name_mangled;
    ASTNode* type_spec;
    ASTNode* value;
};

/* Represents a return statement from a function
 * return x;
 */
struct ReturnStatementNode : public ASTNode {
    void print(const std::string& prepend) override {
        std::printf("%sreturn ", prepend.c_str());
        expr->print("");
        std::printf("\n");
    }

    void set_expr(ExpressionNode* node) {
        expr = node;
    }
    
    void set_expr(ResultNode& expr_node) {
        expr = 
            static_cast<ExpressionNode*>(expr_node.unwrap_or(new ExpressionNode()));
    }

    const ExpressionNode* get_expr() const {
        return expr;
    }

    private:
    ExpressionNode* expr;
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
    void print(const std::string& prepend) override {
        std::printf("%s%s ", prepend.c_str(), tok.name.c_str());
        if (condition != nullptr) {
            condition->print("");
        }

        body->print(prepend);

        if (else_clause != nullptr) {
            else_clause->print(prepend);
        }
        std::printf("\n");
    }

    // @brief Set whether it is an 'if' 'elif' or 'else' clause
    void set_variant(const token& t) {
        tok = t;
    }
    const token& get_variant() const {
        return tok;
    }
    
    void set_condition(ExpressionNode* node) {
        condition = node;
    }
    
    void set_condition(ResultNode& condition_node) {
        condition = 
            static_cast<ExpressionNode*>(condition_node.unwrap_or(new ExpressionNode()));
    }

    const ExpressionNode* get_condition() const {
        return condition;
    }

//    void add_stmt(ASTNode* stmt) {
//        body.push_back(stmt);
//    }
//    const std::vector<ASTNode*>& get_body() const {
//        return body;
//    }
    void set_body(CodeBlockStatementNode* b) {
        body = b;
    }
    CodeBlockStatementNode* get_body() const {
        return body;
    }

    void set_else_clause(ASTNode* clause) {
        else_clause = clause;
    }

    const ASTNode* get_else_clause() const {
        return else_clause;
    }


    private:
    token tok;
    ExpressionNode* condition;  // condition to evaluate 
    // std::vector<ASTNode*> body; // code body
    CodeBlockStatementNode* body;
    ASTNode* else_clause;        // else of elif statement
};

/* Represents a while loop 
 * while (condition) {
 *  ...
 *  }
 */
struct WhileLoopStatementNode : public ASTNode {
    void print(const std::string& prepend) override {
        std::printf("%swhile ", prepend.c_str());
        if (condition != nullptr) {
            condition->print("");
        }
        body->print(prepend);
    }

    void set_condition(ExpressionNode* node) {
        condition = node;
    }
    
    void set_condition(ResultNode& condition_node) {
        condition = 
            static_cast<ExpressionNode*>(condition_node.unwrap_or(new ExpressionNode()));
    }

    const ExpressionNode* get_condition() const {
        return condition;
    }

    void set_body(CodeBlockStatementNode* b) {
        body = b;
    }
    CodeBlockStatementNode* get_body() const {
        return body;
    }

    private:
    ExpressionNode* condition;
    CodeBlockStatementNode* body;
    // std::vector<ASTNode*> body;
};

/* Represents a while loop 
 * while (condition) {
 *  ...
 *  }
 */
struct DoWhileLoopStatementNode : public ASTNode {
    void set_condition(ExpressionNode* node) {
        condition = node;
    }
    
    void set_condition(ResultNode& condition_node) {
        condition = 
            static_cast<ExpressionNode*>(condition_node.unwrap_or(new ExpressionNode()));
    }

    const ExpressionNode* get_condition() const {
        return condition;
    }

    void set_body(CodeBlockStatementNode* b) {
        body = b;
    }
    CodeBlockStatementNode* get_body() const {
        return body;
    }

    void print(const std::string& prepend) override {
        std::printf("%sdo ", prepend.c_str());
        body->print(prepend);
        std::printf("%swhile ", prepend.c_str());
        if (condition != nullptr) {
            condition->print("");
        }
        std::printf("\n");
    }

    private:
    ExpressionNode* condition;
    CodeBlockStatementNode* body;
};

/* Represents a for loop
 * for (init; condition; action) {...}
 */
struct ForLoopStatementNode : public ASTNode {
    // Set the init action
    void set_initialization(ASTNode* init) {
        initialization = init;
    }
    const ASTNode* get_initialization() const {
        return initialization;
    }

    // Condition to loop until
    void set_condition(ExpressionNode* node) {
        condition = node;
    }
    void set_condition(ResultNode& condition_node) {
        condition = 
            static_cast<ExpressionNode*>(condition_node.unwrap_or(new ExpressionNode()));
    }
    const ExpressionNode* get_condition() const {
        return condition;
    }

    void set_body(CodeBlockStatementNode* b) {
        body = b;
    }

    // Add line of code to the body
    void add_stmt(ASTNode* stmt) {
        body->add_stmt(stmt);
        // body.push_back(stmt);
    }
    CodeBlockStatementNode* get_body() const {
        return body;
    }

    // Set the action
    void set_action(ASTNode* init) {
        action = init;
    }
    const ASTNode* get_action() const {
        return action;
    }

    private:
    ASTNode* initialization;    // happens once when loop begins
    ExpressionNode* condition;  // determines when loop breaks when this is false
    ASTNode* action;            // happens at the end of every loop 


    CodeBlockStatementNode* body; // the block of code that executes in the loop
    // std::vector<ASTNode*> body; // the body of code that executes in the loop

    void print(const std::string& prepend) override {
        std::printf("%sfor (", prepend.c_str());
        initialization->print("");
        std::printf("; ");
        condition->print("");
        std::printf("; ");
        action->print("");
        std::printf("(\n");

        body->print(prepend);
    }
};

/* Represents the definition of a struct
 * struct Test {
 *     member :: type;
 * }
 */
struct StructDefinitionNode : public ASTNode {
    void print(const std::string& prepend) override {
        std::printf("%sstruct %s {\n", prepend.c_str(), identifier.name.c_str());
        for (const auto& field : fields) {
            field->print(prepend + "    ");
            std::printf("\n");
        }
        std::printf("}\n");
    }

    void set_identifier(const token& t) {
        identifier = t;
    }
    const token& get_identifier() const {
        return identifier;
    }

    void add_field(ASTNode* field) {
        fields.push_back(field);
    }
    const std::vector<ASTNode*>& get_fields() const {
        return fields;
    }

    private:
    token identifier;
    std::vector<ASTNode*> fields;
};

/* Represents a field member within a struct definition
 * struct Test {
 *     member :: type;
 * }
 */
struct StructMemberFieldNode : public ASTNode {
    void set_identifier(const token& t) {
        identifier = t;
    }
    const token& get_identifier() const {
        return identifier;
    }

    void set_type_spec(ASTNode* ts) {
        type_spec = ts;
    }
    const ASTNode* get_type_spec() const {
        return type_spec;
    }

    void print(const std::string& prepend) override {
        std::printf("%s%s :: %s", prepend.c_str(), identifier.name.c_str(), type_spec->tok.name.c_str());
    }

    private:
    token identifier;
    ASTNode* type_spec;
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
            node->print("");
            std::printf("\n");
        }
    }

    private:
        AST() {}
        std::vector<ASTNode*> nodes;
};

}
