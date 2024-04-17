#pragma once

#include "defines.h"
#include "ast.h"

namespace viper {

struct Symbol {
    Symbol(ASTNode* node);
    ~Symbol();

    std::string 
        name_unmangled,
        name_mangled;
    ASTNode *node;
    bool referenced;
};

}
