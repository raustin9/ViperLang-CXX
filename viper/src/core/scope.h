#pragma once

#include "defines.h"
#include "core/context.h"
#include "core/verror.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <optional>

namespace viper {

struct Symbol;
struct ASTNode;

/*
 * Symbol table or "scope" that a symbol can be defined in
 *
 */
class Scope {
public:
    Scope(
        Scope* parent,
        bool is_module_scope = false
    );
    ~Scope();

    bool add_symbol(Symbol* symbol);
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
