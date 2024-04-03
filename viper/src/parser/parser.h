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

#include <deque>
#include <memory>
#include <unordered_map>
#include <vector>

namespace viper {

class Parser {
    public:
        ~Parser() {}
        static Parser create_new(Tokenizer* lexer);
        std::shared_ptr<AST> parse();

    private:
        using ResultNode = result::Result<ASTNode*, VError>;
        Parser();

        prec_e get_operator_precedence(const token& tok) const;

        result::Result<token, VError> eat(token_kind type);
        token eat();
        token peek_token();
        token current_token();

        ResultNode parse_expr_r(ASTNode* expr, u32 precedence);
        ResultNode parse_expr(u32 precedence = 0);
        ResultNode parse_expr_primary();
//        ResultNode parse_expr_grouping();
//        ResultNode parse_expr_identifier();
        ResultNode parse_expr_boolean();
        ResultNode parse_expr_integer();
        ResultNode parse_expr_float();

        ResultNode parse_statement();
        ResultNode parse_let_statement();
        
        ResultNode parse_struct();
        ResultNode parse_struct_body();
        ResultNode parse_struct_member();


        ResultNode parse_code_block();
        
        ResultNode parse_let_declaration();

        ResultNode parse_procedure();
        ResultNode parse_proc_parameter();

        ResultNode parse_data_type();

        token m_current_token;
        std::unordered_map<token_kind, prec_e> operator_precedences;
        Tokenizer* m_lexer; // [NOT OWNED] 
        std::shared_ptr<AST> m_ast;
        std::vector<VError> error_msgs;
        std::deque<token> token_queue;
};

}
