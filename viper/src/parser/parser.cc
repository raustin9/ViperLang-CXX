#include "parser.h"
#include "core/ast.h"
#include "core/verror.h"
#include "token.h"
#include "tokenizer/tokenizer.h"

namespace viper {

using ResultNode = result::Result<ASTNode*, VError>;

/// @brief Create a new parser from a specified Tokenizer
Parser Parser::create_new(Tokenizer* lexer) {
    Parser p = Parser();
    p.m_lexer = lexer;
    p.m_ast = AST::create_new();

    p.eat();

    return p;
}


/// @brief Parse a code statement
ResultNode Parser::parse_statement() {
    switch(m_current_token.kind) {
        case TK_LET: {
            std::printf("Parsing let statement\n");
            return parse_let_statement();
        } break;
        default:
            return result::Err(VError::create_new(error_type::PARSER_ERR, "Parser::parse_statement: Unexpected token {}", token::kind_to_str(m_current_token.kind)));
    }
}

/// @brief Parse an expression sequence
/// x + 4
/// function_call()
ResultNode Parser::parse_expr() {
    while (m_current_token.kind != TK_SEMICOLON) {
        (void) eat();
    }
    return result::Ok(new ASTNode());
}


// @brief Parse a variable definition. 
// let x: i32 = 4 * 2;
// let y: i32 = x;
ResultNode Parser::parse_let_statement() {
    // Eat the 'let'
    (void) eat(TK_LET);
    
    // Get the variable name
    auto variable_ident_res = eat(TK_IDENT);
    if (variable_ident_res.is_err()) {
        error_msgs.push_back(variable_ident_res.unwrap_err());
    }
    auto variable_ident_tok = variable_ident_res.unwrap_or(token::create_new(TK_IDENT, "__%internal_ident_err", m_current_token.line_num));

    // Eat the ':'
    auto colon_res = eat(TK_COLON);
    if (colon_res.is_err()) {
        auto err = VError::create_new(
            error_type::PARSER_ERR, 
            "Parser::parse_let_statement: expected ':' but got {}.", 
            token::kind_to_str(m_current_token.kind)
        );
        error_msgs.push_back(err);
    }

    // Eat type specifier
    auto typespec_res = parse_data_type();
    if (typespec_res.is_err()) {
        auto err = VError::create_new(
            error_type::PARSER_ERR, 
            "Parser::parse_let_statement: expected return type but got {}.", 
            token::kind_to_str(m_current_token.kind)
        );
        error_msgs.push_back(err);
    }
    auto typespec_node = typespec_res.unwrap();

    // Eat the '='
    auto assign_res = eat(token_kind::TK_ASSIGN);
    if (assign_res.is_err()) {
        auto err = VError::create_new(
            error_type::PARSER_ERR, 
            "Parser::parse_let_statement: expected '=' but got {}.", 
            token::kind_to_str(m_current_token.kind)
        );
        error_msgs.push_back(err);
    }

    // Parse the expression it is being assigned to
    auto expr_res = parse_expr();
    if (expr_res.is_err()) {
        auto err = VError::create_new(
            error_type::PARSER_ERR, 
            "Parser::parse_let_statement: Error parsing expression being assigned"
        );
        error_msgs.push_back(err);
    }
    auto expr_node = expr_res.unwrap_or(
        new ASTNode(
            NodeKind::AST_INVALID_NODE
        )
    );

    (void) eat(TK_SEMICOLON);

    return result::Ok(new VariableDeclaration(
        variable_ident_tok.name,
        typespec_node,
        expr_node
    ));
}


/// @brief Parse a block of code
ResultNode Parser::parse_code_block() {    
    // Eat the "{"
    (void) eat(TK_LSQUIRLY);
    while (m_current_token.kind != TK_RSQUIRLY) {
        auto stmt_res = parse_statement();

    }


    // Eat the "}"
    (void) eat(TK_RSQUIRLY);
    return result::Err(VError::create_new(error_type::PARSER_ERR, "parse_code_block not implemented yet!"));
}

/// @brief Parse a procedure (function)
/// Return the AST node created from it
///
/// proc ident(ident: type [, ident: type]*) {...}
ResultNode Parser::parse_procedure() {
    ProcedureNode *proc_node = new ProcedureNode();
    
    // Eat the 'proc' token 
    auto proc_token = eat(TK_PROC);
    if (proc_token.is_err()) {
        auto proc_err = proc_token.unwrap_err();
        error_msgs.push_back(proc_err);
    }
    token proc_tok = proc_token.unwrap_or(
        token::create_new(TK_PROC, "__%internal_proc_err", m_current_token.line_num)
    );
   
    // Eat the identifier
    auto identifier_res = eat(TK_IDENT);
    if (identifier_res.is_err()) {
        auto identifier_err = identifier_res.unwrap_err();
        error_msgs.push_back(identifier_err);
    }
    token identifier_token = identifier_res.unwrap_or(
        token::create_new(TK_IDENT, "__%internal_ident_err", m_current_token.line_num)
    );
    proc_node->set_name(identifier_token.name);

    // Eat the '('
    (void) eat(TK_LPAREN).unwrap_or(token::create_new(TK_LPAREN, "__%internal_lparen_err", m_current_token.line_num));
    
    // Parse the procedure parameters
    // TODO: THIS IS NOT DONE
    while (m_current_token.kind != TK_RPAREN) {
        auto param = parse_proc_parameter()
            .unwrap_or(nullptr);

        proc_node->add_parameter(param);
        if (m_current_token.kind != TK_COMMA) {
            if (m_current_token.kind == TK_RPAREN) {
                break;
            }
            VError::create_new(
                error_type::PARSER_ERR, 
                "Parser::parse_proc_parameter: expected ',' but got '{}'", 
                token::kind_to_str(m_current_token.kind)
            );
        }
        
        // Eat the ','
        (void) eat(TK_COMMA).unwrap();
    }
   
    // Eat the ')'
    (void) eat(TK_RPAREN).unwrap_or(token::create_new(TK_RPAREN, "__%internal_rparen_err", m_current_token.line_num));

    // Eat the ':'
    (void) eat(TK_COLON).unwrap_or(token::create_new(TK_COLON, "__%internal_colon_err", m_current_token.line_num));

    // Get the return type specification
    auto return_type = parse_data_type().unwrap();
    proc_node->set_return_type(return_type);

    // Parse the code body
    (void) eat(TK_LSQUIRLY);
    while (m_current_token.kind != TK_RSQUIRLY) {
        std::printf("Parsing procedure stmt\n");
        auto stmt_res = parse_statement();
        if (stmt_res.is_err()) {
            error_msgs.push_back(stmt_res.unwrap_err());
        }
        ASTNode* stmt = stmt_res.unwrap_or(
            new ASTNode(NodeKind::AST_INVALID_NODE)
        );
        proc_node->add_statement(stmt);
    }
    // Eat the "}"
    (void) eat(TK_RSQUIRLY);
    
    return result::Ok(proc_node);
}


/// @brief Parse a data type: i32, u8, bool, etc.
ResultNode Parser::parse_data_type() {
    ASTNode* node = new ASTNode();
    auto dt_token = eat(TK_IDENT).unwrap_or(
        token::create_new(TK_IDENT, "__%internal_data_type", m_current_token.line_num)
    );

    return result::Ok(node);
}


/// @brief Parse a parameter for a procedure
/// Return the ASTNode for it if successful, VError otherwise
/// proc ident(ident: type, ident: type) {...}
ResultNode Parser::parse_proc_parameter() {
    ProcParameter* node = new ProcParameter();
    // Eat the first identifier
    auto ident_res = eat(TK_IDENT);
    if (ident_res.is_err()) {
        error_msgs.push_back(ident_res.unwrap_err());
    }
    token identifier_token = ident_res.unwrap_or(
        token::create_new(TK_IDENT, "__%internal_ident_err", m_current_token.line_num)
    );
    node->set_name(identifier_token.name);

    // Eat the ":"
    (void) eat(TK_COLON).unwrap();

    // Eat the type specifier
    auto type_res = eat(TK_IDENT);
    if (type_res.is_err()) {
        error_msgs.push_back(type_res.unwrap_err());
    }
    token type_token = type_res.unwrap_or(
        token::create_new(TK_IDENT, "__%internal_type_err", m_current_token.line_num)
    );
    node->set_data_type(type_token);
    

    return result::Ok(node);
}


/// @brief Parse the tokens and generate the AST
/// @returns the Abstract Syntax tree
std::shared_ptr<AST> Parser::parse() {

    /* Parse at the top level  of file */
    while (m_current_token.kind != TK_EOF) {
        switch (m_current_token.kind) {
            case TK_PROC: {
                auto node = parse_procedure().unwrap();
                m_ast->add_node(node);
                // std::printf("GOT PROC TOKEN\n");
            } break;
//         case TK_PROC: {
// 
//         } break;
//         case TK_PROC: {
// 
//         } break;
//         case TK_PROC: {
// 
//         } break;
//         case TK_PROC: {
// 
//         } break;
//         case TK_PROC: {
// 
//         } break;
//         case TK_PROC: {
// 
//         } break;
//         case TK_PROC: {
// 
//         } break;
            default:
                break;
        }
        // std::printf("TOKEN: %s\n", token::kind_to_str(m_current_token.kind).c_str());
    }


    return m_ast;
}

/// @brief Eat the expected token
token Parser::eat() {
    m_current_token = m_lexer->next_token();
    return m_current_token;
}

/// @brief Eat the expected token
result::Result<token, VError> Parser::eat(token_kind type) {
//    std::printf("Parser::eat got %s\n",
//        token::kind_to_str(m_current_token.kind).c_str()
//    );
    if (m_current_token.kind != type) {
//        std::printf("Parser::eat: Expected %s but got %s\n",
//            token::kind_to_str(type).c_str(),
//            token::kind_to_str(m_current_token.kind).c_str()
//        );
//        
        return result::Err(VError::create_new(
            error_type::PARSER_ERR,
            "Parser::eat: Expected {} but got {}\n",
            token::kind_to_str(type).c_str(),
            token::kind_to_str(m_current_token.kind).c_str()
        ));
    }
    
    m_current_token = m_lexer->next_token();
    return result::Ok(m_current_token);
}

}
