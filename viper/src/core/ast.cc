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


}
