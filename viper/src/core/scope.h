#pragma once

#include "defines.h"
#include "core/context.h"
#include "core/verror.h"
#include "core/vresult.h"

#include <string>
#include <vector>
#include <unordered_map>

namespace viper {

struct Symbol;
struct ASTNode;

class Scope {
    Scope(
        const std::string& description,
        Scope* parent,
        bool is_module_scope = false,
        std::string module = ""
    );
    ~Scope();

    std::string description;
    Scope* parent;
    std::vector<Scope*> scopes;
    bool is_module_scope;
    std::string module_name;
    std::unordered_map<std::string, Symbol*> symbols;

    void add_symbol(Symbol* symbol, Context* context);
    VResult<Symbol*, VError> get_symbol(ASTNode* node);
};

}
