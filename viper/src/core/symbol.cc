#include "symbol.h"

namespace viper {

Symbol::Symbol(ASTNode* node) 
    : node(node), referenced(false)
{

}

Symbol::~Symbol() {

}


}
