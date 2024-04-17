#include "scope.h"

namespace viper {

/// Constructor
Scope::Scope(
    Scope* parent,
    bool _is_module_scope,
    std::string module
) {
    this->parent = parent;
    is_module_scope = _is_module_scope;
    module_name = module;
}

/// Desctructor
Scope::~Scope() {

}

}
