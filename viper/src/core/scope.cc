#include "scope.h"

namespace viper {

/// Constructor
Scope::Scope(
    Scope* parent,
    bool _is_module_scope
) {
    this->parent = parent;
    is_module_scope = _is_module_scope;
}

/// Desctructor
Scope::~Scope() {

}


/// @brief Add a symbol to the symbol table
/// @returns False if the symbol collides
///          True otherwise
bool Scope::add_symbol(Symbol* sym) {
    return false;
}

}
