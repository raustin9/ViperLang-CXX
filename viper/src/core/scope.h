#pragma once

#include "defines.h"
#include "core/context.h"
#include "core/verror.h"

#include <string>
#include <vector>
#include <unordered_map>

namespace viper {

struct Symbol;
struct ASTNode;

class Scope {
public:
    Scope(
        Scope* parent,
        bool is_module_scope = false,
        std::string module = ""
    );
    ~Scope();

    void add_symbol(Symbol* symbol, Context* context);
    std::optional<Symbol*> get_symbol(ASTNode* node);
private:
    Scope* parent;
    bool is_module_scope;
    std::string module_name;
    std::string description;
    std::vector<Scope*> scopes;
    std::unordered_map<std::string, Symbol*> symbols;

};

}
