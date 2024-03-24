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
#include "core/vresult.h"
#include "core/result.h"
#include "core/verror.h"

#include <memory>
#include <vector>

namespace viper {

class ParserError : public VError {
    public:
        

    private:
};

class Parser {
    public:
        ~Parser() {}
        static Parser create_new(Tokenizer* lexer);
        std::shared_ptr<AST> parse();

    private:
        Parser() {}

        VResult<token, ParserError> eat(token_kind type);
        token eat();

        result::Result<ASTNode*, ParserError> parse_procedure();
        result::Result<ASTNode*, ParserError> parse_proc_parameter();
        result::Result<ASTNode*, ParserError> parse_data_type();

        token m_current_token;
        Tokenizer* m_lexer; // [NOT OWNED] 
        std::shared_ptr<AST> m_ast;
};

}
