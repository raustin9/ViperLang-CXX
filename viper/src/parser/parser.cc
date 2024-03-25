#include "parser.h"
#include "core/ast.h"
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
    // TODO: NOT IMPLEMENTED
    (void) eat(TK_LSQUIRLY);
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
result::Result<token, VError> Parser::eat(token_kind type) {
    std::printf("Parser::eat got %s\n",
        token::kind_to_str(m_current_token.kind).c_str()
    );
    if (m_current_token.kind != type) {
        std::printf("Parser::eat: Expected %s but got %s\n",
            token::kind_to_str(type).c_str(),
            token::kind_to_str(m_current_token.kind).c_str()
        );
        
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
