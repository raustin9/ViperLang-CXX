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

std::string& ProcedureNode::get_name() {
    return name;
}

void ProcedureNode::set_name(const std::string& name) {
    this->name = name;
}

}
