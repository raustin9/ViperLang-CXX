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


}
