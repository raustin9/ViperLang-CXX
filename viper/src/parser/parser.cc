#include "parser.h"
#include "core/ast.h"
#include "core/verror.h"
#include "token.h"
#include "tokenizer/tokenizer.h"

namespace viper {

using ResultNode = result::Result<ASTNode*, VError>;
using ResultToken = result::Result<token, VError>;

Parser::Parser() {
    operator_precedences[TK_ASSIGN] = precedence::ASSIGN;
    operator_precedences[TK_TIMESEQ] = precedence::ASSIGN;
    operator_precedences[TK_DIVEQ] = precedence::ASSIGN;
    operator_precedences[TK_MODEQ] = precedence::ASSIGN;
    operator_precedences[TK_EQUALTO] = precedence::COMPARISON;
    operator_precedences[TK_PLUSEQ] = precedence::COMPARISON;
    operator_precedences[TK_MINUSEQ] = precedence::COMPARISON;
    operator_precedences[TK_LT] = precedence::COMPARISON;
    operator_precedences[TK_GT] = precedence::COMPARISON;
    operator_precedences[TK_LTEQ] = precedence::COMPARISON;
    operator_precedences[TK_GTEQ] = precedence::COMPARISON;
    operator_precedences[TK_PLUS] = precedence::ADDSUB;
    operator_precedences[TK_MINUS] = precedence::ADDSUB;
    operator_precedences[TK_ASTERISK] = precedence::MULDIVMOD;
    operator_precedences[TK_MOD] = precedence::MULDIVMOD;
    operator_precedences[TK_SLASH] = precedence::MULDIVMOD;
    operator_precedences[TK_BANG] = precedence::PREFIX;
    operator_precedences[TK_TILDE] = precedence::PREFIX;
}

precedence Parser::get_operator_precedence(const token& tok) const {
    if (operator_precedences.find(tok.kind) != operator_precedences.end()) {
        return operator_precedences.find(tok.kind)->second;
    }

    return precedence::INVALID_OP;
}

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
            // std::printf("Parsing let statement\n");
            return parse_let_statement();
        } break;
        default:
            return result::Err(VError::create_new(error_type::PARSER_ERR, "Parser::parse_statement: Unexpected token {}", token::kind_to_str(m_current_token.kind)));
    }
}


ResultNode Parser::parse_expr_primary() {
    switch(m_current_token.kind) {
        case token_kind::TK_TRUE: return parse_expr_boolean();
        case token_kind::TK_FALSE: return parse_expr_boolean();
        case token_kind::TK_NUM_FLOAT: return parse_expr_float();
        case token_kind::TK_NUM_INT: return parse_expr_integer();
        
        case token_kind::TK_LPAREN: return result::Err(VError::create_new(error_type::PARSER_ERR, "Parser::parse_expr_primary: parsing grouping expression not implemented yet :(", token::kind_to_str(m_current_token.kind)));
        case token_kind::TK_IDENT: return result::Err(VError::create_new(error_type::PARSER_ERR, "Parser::parse_expr_primary: parsing identifiers not implemented yet :(", token::kind_to_str(m_current_token.kind)));
        case token_kind::TK_STR: return result::Err(VError::create_new(error_type::PARSER_ERR, "Parser::parse_expr_primary: parsing string literals not implemented yet :(", token::kind_to_str(m_current_token.kind)));
        default:
            return result::Err(VError::create_new(error_type::PARSER_ERR, "Parser::parse_expr_primary: Invalid token '{}' when parsing primary expression", token::kind_to_str(m_current_token.kind)));
    }
}


/// @brief Parse a true or false boolean expression
ResultNode Parser::parse_expr_boolean() {
    token bool_tok;
    switch(m_current_token.kind) {
        case TK_TRUE:
            bool_tok = eat(TK_TRUE).unwrap();
            return result::Ok(new BooleanLiteralNode(true));
            break;
        case TK_FALSE:
            bool_tok = eat(TK_FALSE).unwrap();
            return result::Ok(new BooleanLiteralNode(false));
            break;
        default:
            return result::Err(VError::create_new(error_type::PARSER_ERR, "Parser::parse_expr_boolean: expected true or false. Got {}", m_current_token.name));
    }
}

/// @brief Parse an integer literal expression
ResultNode Parser::parse_expr_integer() {
    token int_tok = m_current_token;
    auto integer_res = eat(TK_NUM_INT);
    integer_res.unwrap(); // we should only get here if a parent function read an integer literal,


    u64 value = std::atoi(int_tok.name.c_str());
    return result::Ok(new IntegerLiteralNode(value));
}


/// @brief Parse a float literal expression
ResultNode Parser::parse_expr_float() {
    token fp_tok = m_current_token;
    auto float_res = eat(TK_NUM_FLOAT);
    token float_tok = float_res.unwrap();

    f64 value = std::atof(fp_tok.name.c_str());

    return result::Ok(new FloatLiteralNode(value));
}

/// @brief Parse an expression sequence
/// literal -> 4 | "hello, world" | ...keywords
/// unary -> ~var | !var
/// grouping -> "(" expression ")"
ResultNode Parser::parse_expr(prec_e precedence) {
    token prefix = m_current_token;
    ResultNode r_LHS;
    
    if (get_operator_precedence(prefix) != precedence::INVALID_OP) {
        std::printf("Prefix operator\n");
        r_LHS = parse_expr_prefix();
    } else {
        r_LHS = parse_expr_primary();
    }
    

    r_LHS = parse_expr_r(r_LHS.unwrap(), precedence);

    return result::Ok(r_LHS.unwrap());
}


/// @brief Parse an expression with an infix operator
/// x + y
/// w == true
ResultNode Parser::parse_expr_infix(ExpressionNode* lhs) {
    ExpressionBinaryNode* expr = new ExpressionBinaryNode();

    expr->lhs = lhs;

    // Get and eat the operator
    token op = m_current_token;
    prec_e prec = get_operator_precedence(op);
    (void) eat();
    
    ResultNode r_RHS = parse_expr(prec);
    if (r_RHS.is_err()) {
        return result::Err(VError::create_new(error_type::PARSER_ERR, "Error parsing LHS expression parsing infix epxr"));
    }
    expr->rhs = static_cast<ExpressionNode*>(r_RHS.unwrap());

    return result::Ok(expr);
}


/// @brief Parse an expression with a prefix operator
/// !x
/// ~x
ResultNode Parser::parse_expr_prefix() {
    ExpressionPrefixNode* expr = new ExpressionPrefixNode();
    token prefix = m_current_token;
    if (get_operator_precedence(prefix) != precedence::PREFIX) {
        return result::Err(VError::create_new(error_type::PARSER_ERR, "Invalid prefix operator {}. Did you mean ! or ~?", token::kind_to_str(prefix.kind)));
    }
    // token prefix = eat(); // eat the operator token
   
    expr->op = prefix;
    
    ResultNode r_RHS = parse_expr(precedence::PREFIX);
    expr->rhs = static_cast<ExpressionNode*>(r_RHS.unwrap());

    return result::Ok(expr);
}


ResultNode Parser::parse_expr_r(ASTNode* expr, prec_e precedence) {
    token op = m_current_token;
   
    // Get operator precedence. 
    // If negative, the token 
    // is not valid operator so return expression
    prec_e prec = get_operator_precedence(op);
    if (prec == precedence::INVALID_OP) {
        // std::string str = std::format("Got non-op: {}", token::kind_to_str(op.kind));
        // std::cout << str << "\n";
        return result::Ok(expr);
    }

    // Eat the operator
    (void) eat();

    ResultNode r_RHS = parse_expr_primary();

    prec_e next_prec = get_operator_precedence(m_current_token);
    if (next_prec == precedence::INVALID_OP) {
        // TODO: returna binary expression with the LHS and RHS
        return result::Ok(expr);
    }

    return result::Ok(expr);
}


// @brief Parse a variable definition. 
// let x: i32 = 4 * 2;
// let y: i32 = x;
ResultNode Parser::parse_let_statement() {
    // Eat the 'let'
    (void) eat(TK_LET);
    
    // Get the variable name
    token id_tok = m_current_token;
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
    auto expr_res = parse_expr(precedence::LOWEST);
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
        id_tok.name,
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
    token proc_token = m_current_token;
    auto r_proc =  eat(TK_PROC);
    if (r_proc.is_err()) {
        auto proc_err = r_proc.unwrap_err();
        error_msgs.push_back(proc_err);
    }
    token proc_tok = r_proc.unwrap_or(
        token::create_new(TK_PROC, "__%internal_proc_err", m_current_token.line_num)
    );
   
    // Eat the identifier
    token ident_tok = m_current_token;
    auto identifier_res = eat(TK_IDENT);
    if (identifier_res.is_err()) {
        auto identifier_err = identifier_res.unwrap_err();
        error_msgs.push_back(identifier_err);
    }
    proc_node->set_name(ident_tok.name);

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
        // std::printf("Parsing procedure stmt\n");
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
    token dt_tok = m_current_token;
    auto r_dt_tok = eat(TK_IDENT).unwrap_or(
        token::create_new(TK_IDENT, "__%internal_data_type", m_current_token.line_num)
    );
    node->tok = dt_tok;

    return result::Ok(node);
}


/// @brief Parse a parameter for a procedure
/// Return the ASTNode for it if successful, VError otherwise
/// proc ident(ident: type, ident: type) {...}
ResultNode Parser::parse_proc_parameter() {
    ProcParameter* node = new ProcParameter();
    token id_tok = m_current_token;
    
    // Eat the first identifier
    auto ident_res = eat(TK_IDENT);
    if (ident_res.is_err()) {
        error_msgs.push_back(ident_res.unwrap_err());
    }
    token identifier_token = ident_res.unwrap_or(
        token::create_new(TK_IDENT, "__%internal_ident_err", m_current_token.line_num)
    );
    node->set_name(id_tok.name);

    // Eat the ":"
    (void) eat(TK_COLON).unwrap();

    // Eat the type specifier
    token type_tok = m_current_token;
    auto type_res = eat(TK_IDENT);
    if (type_res.is_err()) {
        error_msgs.push_back(type_res.unwrap_err());
    }
    node->set_data_type(type_tok);

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
            } break;
            case TK_LET: {
                auto node = parse_let_statement().unwrap();
                m_ast->add_node(node);
            } break;
            default:
                break;
        }
        // std::printf("TOKEN: %s\n", token::kind_to_str(m_current_token.kind).c_str());
    }


    return m_ast;
}


/// @brief Look at the next token in the stream without advancing the current token
token Parser::peek_token() {
    token_queue.push_back(m_current_token);
   
    return m_current_token;
}


/// @brief Return the current token
token Parser::current_token() {
    return m_current_token;
    // return token_queue[0];
}

/// @brief Eat the expected token
token Parser::eat() {
    m_current_token = m_lexer->next_token();
    return m_current_token;
}

/// @brief Eat the expected token
result::Result<token, VError> Parser::eat(token_kind type) {
    if (m_current_token.kind != type) {
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
