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

class Parser {
    public:
        ~Parser() {}
        static Parser create_new(Tokenizer* lexer);
        std::shared_ptr<AST> parse();

    private:
        using ResultNode = result::Result<ASTNode*, VError>;
        Parser() {}

        result::Result<token, VError> eat(token_kind type);
        token eat();
        
        ResultNode parse_struct();
        ResultNode parse_struct_body();
        ResultNode parse_struct_member();

        ResultNode parse_let_declaration();

        ResultNode parse_procedure();
        ResultNode parse_proc_parameter();

        ResultNode parse_data_type();

        token m_current_token;
        Tokenizer* m_lexer; // [NOT OWNED] 
        std::shared_ptr<AST> m_ast;
        std::vector<VError> error_msgs;
};

}
