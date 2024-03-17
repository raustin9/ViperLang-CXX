#pragma once

/*
 *  parser.h
 *
 *  This contains the interface of the Parser for the Viper language
 */

#include "defines.h"
#include "tokenizer/tokenizer.h"
#include "token.h"
#include "core/ast.h"

#include <memory>
#include <vector>

namespace viper {

class Parser {
    public:
        ~Parser() {}
        static Parser create_new(Tokenizer* lexer);
        std::shared_ptr<AST> parse();

    private:
        Parser() {}

        token eat(token_kind type);

        token m_current_token;
        Tokenizer* m_lexer; // [NOT OWNED]        
};

}
