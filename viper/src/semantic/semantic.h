#pragma once

#include "defines.h"
#include <memory>
#include "core/ast.h"

namespace viper {

class SemanticAnalyzer {
    public:
        SemanticAnalyzer(std::shared_ptr<AST> tree)
            : ast(tree) {}
        ~SemanticAnalyzer() {}

        void analyze_node(ASTNode* node);
        void analyze_ast();

    private:
        std::shared_ptr<AST> ast;
};

} // viper namespace
