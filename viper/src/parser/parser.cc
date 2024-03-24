#include "parser.h"
#include "core/ast.h"
#include "token.h"
#include "tokenizer/tokenizer.h"

namespace viper {

/// @brief Create a new parser from a specified Tokenizer
Parser Parser::create_new(Tokenizer* lexer) {
    Parser p = Parser();
    p.m_lexer = lexer;
    p.m_ast = AST::create_new();

    p.eat();

    return p;
}

/// @brief Parse a procedure (function)
/// Return the AST node created from it
///
/// proc ident(ident: type [, ident: type]*) {...}
result::Result<ASTNode*, ParserError> Parser::parse_procedure() {
    ProcedureNode *proc_node = new ProcedureNode();
    
    // Eat the 'proc' token 
    auto proc_token = eat(TK_PROC).unwrap_or(token::create_new(TK_PROC, "__%internal_proc_err", m_current_token.line_num));
   
    // Eat the identifier
    auto identifier_token = eat(TK_IDENT).unwrap_or(token::create_new(TK_IDENT, "__%internal_ident_err", m_current_token.line_num));
    proc_node->set_name(identifier_token.name);

    // Eat the '('
    (void) eat(TK_LPAREN).unwrap_or(token::create_new(TK_LPAREN, "__%internal_lparen_err", m_current_token.line_num));
    
    // Parse the procedure parameters
    // TODO: THIS IS NOT DONE
    while (m_current_token.kind != TK_RPAREN) {
        auto param = parse_proc_parameter()
            .unwrap_or(nullptr);

        proc_node->add_parameter(param);
    }
   
    // Eat the ')'
    (void) eat(TK_RPAREN).unwrap_or(token::create_new(TK_RPAREN, "__%internal_rparen_err", m_current_token.line_num));

    // Eat the ':'
    (void) eat(TK_COLON).unwrap_or(token::create_new(TK_COLON, "__%internal_colon_err", m_current_token.line_num));

    // Get the return type specification
    // auto return_type = parse_data_type();
    (void)parse_data_type();

    // Parse the code body
    // TODO: NOT IMPLEMENTED
    (void) eat(TK_LSQUIRLY);
    (void) eat(TK_RSQUIRLY);
    
    return result::Ok(proc_node);
}


/// @brief Parse a data type: i32, u8, bool, etc.
VResult<ASTNode*, VError> Parser::parse_data_type() {
    eat();
    return {};
}




/// @brief Parse a parameter for a procedure
/// Return the ASTNode for it if successful, VError otherwise
/// proc ident(ident: type, ident: type) {...}
VResult<ASTNode*, VError> Parser::parse_proc_parameter() {
    return {};
}


/// @brief Parse the tokens and generate the AST
/// @returns the Abstract Syntax tree
std::shared_ptr<AST> Parser::parse() {

    /* Parse at the top level  of file */
    while (m_current_token.kind != TK_EOF) {
        switch (m_current_token.kind) {
            case TK_PROC: {
                auto node = parse_procedure();
                std::printf("GOT PROC TOKEN\n");
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
        std::printf("TOKEN: %s\n", token::kind_to_str(m_current_token.kind).c_str());
    }


    return {};
}

/// @brief Eat the expected token
token Parser::eat() {
    m_current_token = m_lexer->next_token();
    return m_current_token;
}

/// @brief Eat the expected token
VResult<token, VError> Parser::eat(token_kind type) {
    std::printf("Parser::eat got %s\n",
        token::kind_to_str(m_current_token.kind).c_str()
    );
    if (m_current_token.kind != type) {
        std::printf("Parser::eat: Expected %s but got %s\n",
            token::kind_to_str(type).c_str(),
            token::kind_to_str(m_current_token.kind).c_str()
        );
       
        // @bad -- Change this function to use a VResult or VError type
        // to handle potential errors
        exit(1);
    }

    
    
    m_current_token = m_lexer->next_token();
    return VResult<token, VError>::Ok(m_current_token);
}

}
