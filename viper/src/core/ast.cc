#include "ast.h"
#include <memory>

namespace viper {

    std::shared_ptr<ASTNode> ASTNode::create_new(token tok, NodeKind kind) {
        auto node = std::make_shared<ASTNode>(ASTNode());
        node->kind = kind;
        node->tok = tok;
        return node;
    }

    std::shared_ptr<AST> AST::create_new() {
        auto ast = std::make_shared<AST>(AST());
        return ast;
    }


/// PROCEDURE NODE ///
ProcedureNode::ProcedureNode() {
    scope = new Scope(nullptr);
}

ProcedureNode::~ProcedureNode() {
    delete scope;
}


/// @brief Add a statement to the body of the procedure
void ProcedureNode::add_statement(ASTNode* stmt) {
    stmt->parent = this;
    // code_body.push_back(stmt);
}

/// @return Get the name of the procedure
std::string& ProcedureNode::get_name() {
    return name;
}


/// @brief Set the name of the procedure
void ProcedureNode::set_name(const std::string& name) {
    this->name = name;
}


/// @brief Add a new parameter to the list of parameters for this procedure
/// @param node Pointer to the AST node of the parameter we want to add
void ProcedureNode::add_parameter(ASTNode* node) {
    parameters.push_back(node);
}


/// @brief Set the return declarator of the function to this procedure
void ProcedureNode::set_return_type(ASTNode* node) {
    this->return_declarator = node;
}


/// @return The return declarator node for the procedure
const ASTNode* ProcedureNode::get_return_type() const {
    return this->return_declarator;
}


///// Code Block ///
//void ScopeNode::add_statement(ASTNode* node) {
//    this->statements.push_back(node);
//}
//
//const std::vector<ASTNode*>& ScopeNode::get_statements() {
//    return statements;
//}


///// @brief Set the parent AST node of this code block
//void ScopeNode::set_parent(ASTNode* node) {
//    parent = node;
//}
//
//
///// @brief Get the parent node of this code block. The parent is who this block belongs to
//const ASTNode* ScopeNode::get_parent() {
//    return parent;
//}


/// AST ///

/// @brief Get the ast nodes from the tree
const std::vector<ASTNode*>& AST::get_nodes() const {
    return this->nodes;
}



/// @brief Add a node to the tree
void AST::add_node(ASTNode* node) {
    nodes.push_back(node);
}

}
