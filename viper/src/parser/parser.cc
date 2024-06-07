#include "parser.h"
#include "core/ast.h"
#include "core/verror.h"
#include "token.h"
#include "tokenizer/tokenizer.h"
#include <iostream>

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
    operator_precedences[TK_LOG_OR] = precedence::LOGICAL_OR_AND;
    operator_precedences[TK_LOG_AND] = precedence::LOGICAL_OR_AND;
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

/// @brief Get the precedence of the operator being 
precedence Parser::get_operator_precedence(const token& tok) const {
    if (operator_precedences.find(tok.kind) != operator_precedences.end()) {
        return operator_precedences.find(tok.kind)->second;
    }

    return precedence::INVALID_OP;
}

/// @brief Determine if token is a valid prefix operator
/// @returns Precedence if it is, INVALID_OP if not
prec_e Parser::is_prefix_op(const token& tok) const {
    switch (tok.kind) {
        case TK_TILDE:
        case TK_MINUS:
        case TK_BANG:
            return precedence::PREFIX;
        default:
            return precedence::INVALID_OP;
    }
}

/// @brief Create a new parser from a specified Tokenizer
Parser Parser::create_new(Tokenizer* lexer) {
    Parser p = Parser();
    p.m_lexer = lexer;
    p.m_ast = AST::create_new();

    p.m_peek_token = p.m_lexer->next_token();
    p.eat();
    // std::printf("1st token: %s\n", token::kind_to_str(p.m_current_token.kind).c_str());

    return p;
}


/// @brief Parse a code statement
ResultNode Parser::parse_statement() {
    switch(m_current_token.kind) {
        case TK_LET: {
            // std::printf("Parsing let statement\n");
            ResultNode node = parse_let_statement();
            (void) eat(TK_SEMICOLON);
            return node;
        } break;
        case TK_RETURN: {
            ResultNode node = parse_return_statement();
            (void) eat(TK_SEMICOLON);
            return node;
        } break;
        case TK_IF: {
            return parse_if_statement();
        } break;
        case TK_WHILE: {
            return parse_while_statement();
        } break;
        case TK_DO: {
            ResultNode node = parse_do_while_statement();
            (void) eat(TK_SEMICOLON);
            return node;
        } break;
        case TK_FOR: {
            return parse_for_statement();
        } break;
        default:
            ResultNode node = parse_expression_statement();
            (void) eat(TK_SEMICOLON);
            return node;
    }
}


/// @brief Parse a code block statement
/// {
///      let i: i32 = foo();
///      bar();
///      ...
/// }
ResultNode Parser::parse_scope() {
    CodeBlockStatementNode* scope = new CodeBlockStatementNode();
    (void) eat(TK_LSQUIRLY);

    // Parse all the statements
    while (m_current_token.kind != TK_RSQUIRLY) {
        auto stmt_res = parse_statement();
        if (stmt_res.is_err()) {
            std::cout << "2" << std::endl;
            error_msgs.push_back(stmt_res.unwrap_err());
        }
        ASTNode* stmt = stmt_res.unwrap_or(
            new ASTNode(NodeKind::AST_INVALID_NODE)
        );
        scope->add_stmt(stmt);
    }

    (void) eat(TK_RSQUIRLY);
    return result::Ok(scope);
}


/// @brief Parse an if statement
/// Represents a conditional statement
/// if (condition) {
///  ...
/// } elif (condition) {
///  ...
/// } else {
///  ...
/// }
ResultNode Parser::parse_if_statement() {
    ConditionalStatementNode* condition_node = new ConditionalStatementNode();
    // condition_node->tok = m_current_token;
    condition_node->set_variant(m_current_token);

    (void) eat(TK_IF);

    ResultNode r_condition = parse_expr();
    if (r_condition.is_err()) {
        error_msgs.push_back(
            VError::create_new(
                error_type::PARSER_ERR,
                "Parser::parse_if_statement: unable to parse condition expression"
            )
        );
    }
    ExpressionNode* condition = 
        static_cast<ExpressionNode*>(r_condition.unwrap_or(new ExpressionNode()));
    // condition_node->condition = condition;
    condition_node->set_condition(condition);

    // Parse the code body
    ResultNode r_body = parse_scope();
    if (r_body.is_err()) {
        error_msgs.push_back(r_body.unwrap_err());
    }
    CodeBlockStatementNode* body = 
        static_cast<CodeBlockStatementNode*>(r_body.unwrap_or(new CodeBlockStatementNode()));
    condition_node->set_body(body);

    switch (m_current_token.kind) {
        case TK_ELIF: { // elif clause
            ResultNode r_elif_node = parse_elif_statement();
            if (r_elif_node.is_err()) {
                error_msgs.push_back(r_elif_node.unwrap_err());
            }
            ConditionalStatementNode* elif_node = 
                static_cast<ConditionalStatementNode*>(r_elif_node.unwrap_or(new ConditionalStatementNode()));

            condition_node->set_else_clause(elif_node);
            // condition_node->else_claus = elif_node;
        } break;
        case TK_ELSE: { // else clause
            ResultNode r_else_node = parse_else_statement();
            if (r_else_node.is_err()) {
                error_msgs.push_back(r_else_node.unwrap_err());
            }
            ConditionalStatementNode* else_node = 
                static_cast<ConditionalStatementNode*>(r_else_node.unwrap_or(new ConditionalStatementNode()));

            condition_node->set_else_clause(else_node);
            // condition_node->else_claus = else_node;
        } break;
        default:
            condition_node->set_else_clause(nullptr);
            // condition_node->else_claus = nullptr;
            break;
    }

    return result::Ok(condition_node);
}


/// @brief Parse a while loop statement
ResultNode Parser::parse_while_statement() {
    WhileLoopStatementNode* while_loop_node = new WhileLoopStatementNode();
    (void) eat(TK_WHILE);

    ResultNode r_condition = parse_expr();
    if (r_condition.is_err()) {
        error_msgs.push_back(
            VError::create_new(
                error_type::PARSER_ERR,
                "Parser::parse_if_statement: unable to parse condition expression"
            )
        );
    }
    ExpressionNode* condition = 
        static_cast<ExpressionNode*>(r_condition.unwrap_or(new ExpressionNode()));
    while_loop_node->set_condition(condition);
    // while_loop_node->condition = condition;

    // Parse the code body
    ResultNode r_body = parse_scope();
    if (r_body.is_err()) {
        error_msgs.push_back(r_body.unwrap_err());
    }
    CodeBlockStatementNode* body = 
        static_cast<CodeBlockStatementNode*>(r_body.unwrap_or(new CodeBlockStatementNode()));
    while_loop_node->set_body(body);
    
    return result::Ok(while_loop_node);
}


/// @brief Parse an expression statement
/// i = 22 + 4;
ResultNode Parser::parse_expression_statement() {
    ExpressionStatementNode* expr_stmt = new ExpressionStatementNode();
    ResultNode r_expr = parse_expr();
    if (r_expr.is_err()) {
        error_msgs.push_back(r_expr.unwrap_err());
    }
    ExpressionNode* expr = 
        static_cast<ExpressionNode*>(r_expr.unwrap_or(new ExpressionNode()));

    expr_stmt->set_expr(expr);
    return result::Ok(expr_stmt);
}

/// @brief Parse a do-while loop statement
/// do {
///     ...
/// } while (condition);
ResultNode Parser::parse_do_while_statement() {
    DoWhileLoopStatementNode* do_while_node = new DoWhileLoopStatementNode();
    (void) eat(TK_DO);

    // Parse code block body
    ResultNode r_body = parse_scope();
    if (r_body.is_err()) {
        error_msgs.push_back(r_body.unwrap_err());
    }
    CodeBlockStatementNode* body = 
        static_cast<CodeBlockStatementNode*>(r_body.unwrap_or(new CodeBlockStatementNode()));
    do_while_node->set_body(body);

    (void) eat(TK_WHILE);
    
    ResultNode r_condition = parse_expr();
    if (r_condition.is_err()) {
        error_msgs.push_back(
            VError::create_new(
                error_type::PARSER_ERR,
                "Parser::parse_if_statement: unable to parse condition expression"
            )
        );
    }
    ExpressionNode* condition = 
        static_cast<ExpressionNode*>(r_condition.unwrap_or(new ExpressionNode()));
    do_while_node->set_condition(condition);

    return result::Ok(do_while_node);
}


/// @brief Parse a for loop statement
/// for (init; condition; action) {...}
/// for (let i: i32 = 0; i < 10; i += 1) {...}
ResultNode Parser::parse_for_statement() {
    ForLoopStatementNode* for_node = new ForLoopStatementNode();
    (void) eat(TK_FOR);
    (void) eat(TK_LPAREN);

    // Parse the initialization
    ResultNode r_init_node = parse_statement();
    if (r_init_node.is_err()) {
        error_msgs.push_back(r_init_node.unwrap_err());
    }
    ASTNode* init_node = r_init_node.unwrap_or(new ASTNode());

    // Parse the condition
    ResultNode r_condition = parse_expr();
    if (r_init_node.is_err()) {
        error_msgs.push_back(r_init_node.unwrap_err());
    }
    ExpressionNode* condition = 
        static_cast<ExpressionNode*>(r_condition.unwrap_or(new ExpressionNode()));
    (void) eat(TK_SEMICOLON);

    // Parse the action
    ResultNode r_action_node = parse_expression_statement();
    if (r_action_node.is_err()) {
        error_msgs.push_back(r_action_node.unwrap_err());
    }
    ASTNode* action_node = r_action_node.unwrap_or(new ASTNode());
    (void) eat(TK_RPAREN);

    // Parse code body
    ResultNode r_body = parse_scope();
    if (r_body.is_err()) {
        std::cout << "1" << std::endl;
        error_msgs.push_back(r_body.unwrap_err());
    }
    CodeBlockStatementNode* body = 
        static_cast<CodeBlockStatementNode*>(r_body.unwrap_or(new CodeBlockStatementNode()));

    for_node->set_initialization(init_node);
    for_node->set_condition(condition);
    for_node->set_action(action_node);
    for_node->set_body(body);

    return result::Ok(for_node);
}


/// @brief Parse elif clause portion of a conditional
ResultNode Parser::parse_elif_statement() {
    ConditionalStatementNode* elif_node = new ConditionalStatementNode();
    elif_node->set_variant(m_current_token);
    (void) eat(TK_ELIF);

    ResultNode r_condition = parse_expr();
    if (r_condition.is_err()) {
        error_msgs.push_back(
            VError::create_new(
                error_type::PARSER_ERR,
                "Parser::parse_if_statement: unable to parse condition expression"
            )
        );
    }
    ExpressionNode* condition = 
        static_cast<ExpressionNode*>(r_condition.unwrap_or(new ExpressionNode()));
    elif_node->set_condition(condition);

    // Parse the code body
    ResultNode r_body = parse_scope();
    if (r_body.is_err()) {
        error_msgs.push_back(r_body.unwrap_err());
    }
    CodeBlockStatementNode* body = 
        static_cast<CodeBlockStatementNode*>(r_body.unwrap_or(new CodeBlockStatementNode()));
    elif_node->set_body(body);

    switch (m_current_token.kind) {
        case TK_ELIF: { // elif clause
            ResultNode r_elif_node = parse_elif_statement();
            if (r_elif_node.is_err()) {
                error_msgs.push_back(r_elif_node.unwrap_err());
            }
            ConditionalStatementNode* node = 
                static_cast<ConditionalStatementNode*>(r_elif_node.unwrap_or(new ConditionalStatementNode()));

            elif_node->set_else_clause(node);
        } break;
        case TK_ELSE: { // else clause
            ResultNode r_elif_node = parse_else_statement();
            if (r_elif_node.is_err()) {
                error_msgs.push_back(r_elif_node.unwrap_err());
            }
            ConditionalStatementNode* node = 
                static_cast<ConditionalStatementNode*>(r_elif_node.unwrap_or(new ConditionalStatementNode()));

            elif_node->set_else_clause(node);
        } break;
        default:
            elif_node->set_else_clause(nullptr);
            break;
    }
    return result::Ok(elif_node);
}


/// @brief Parse elif clause portion of a conditional
ResultNode Parser::parse_else_statement() {
    ConditionalStatementNode* else_node = new ConditionalStatementNode();
    else_node->set_variant(m_current_token);
    // else_node->tok = m_current_token;
    (void) eat(TK_ELSE);

    // No condition for else claus
    else_node->set_condition(nullptr);
    // else_node->condition = nullptr;

    // Parse the code body
    ResultNode r_body = parse_scope();
    if (r_body.is_err()) {
        error_msgs.push_back(r_body.unwrap_err());
    }
    CodeBlockStatementNode* body = 
        static_cast<CodeBlockStatementNode*>(r_body.unwrap_or(new CodeBlockStatementNode()));
    else_node->set_body(body);
    
    else_node->set_else_clause(nullptr);

    // Else marks the end of the conditional chain -- no further parsing

    return result::Ok(else_node);
}


/// @brief Parse the return statement
ResultNode Parser::parse_return_statement() {
    ReturnStatementNode* return_node = new ReturnStatementNode();
    (void) eat(TK_RETURN);
    ResultNode r_expr = parse_expr();
    if (r_expr.is_err()) {
        error_msgs.push_back(
            VError::create_new(
                error_type::PARSER_ERR,
                "Parser::parse_return_statement: unable to parse expression!"
            )
        );
    }
    ExpressionNode* expr = static_cast<ExpressionNode*>(r_expr.unwrap_or(new ExpressionNode()));
    return_node->set_expr(expr);
    // return_node->expr = expr;
    return result::Ok(return_node);
}


ResultNode Parser::parse_expr_primary() {
    switch(m_current_token.kind) {
        // Unary or prefix expressions
        case token_kind::TK_BANG:
        case token_kind::TK_MINUS:
        case token_kind::TK_TILDE:
            return parse_expr_prefix();

        // Boolean Expression
        case token_kind::TK_TRUE: return parse_expr_boolean();
        case token_kind::TK_FALSE: return parse_expr_boolean();
       
        // Numbers
        case token_kind::TK_NUM_FLOAT: return parse_expr_float();
        case token_kind::TK_NUM_INT: return parse_expr_integer();
        
        case token_kind::TK_IDENT: return parse_expr_identifier();
        
        case token_kind::TK_LPAREN: return parse_expr_grouping();
        case token_kind::TK_STR: return parse_expr_str();
        default:
            return result::Err(VError::create_new(error_type::PARSER_ERR, "Parser::parse_expr_primary: Invalid token '{}' when parsing primary expression", token::kind_to_str(m_current_token.kind)));
    }
}


/// @brief Parse a string literal expression
/// "string example"
ResultNode Parser::parse_expr_str() {
    ExpressionStringLiteralNode* str_expr = new ExpressionStringLiteralNode();
    token str = m_current_token;
    (void) eat(TK_STR);
   
    str_expr->set_token(str);
    //str_expr->tok = str;

    return result::Ok(str_expr);
}



/// @brief Parse a grouping expression.
///        (1 + 3)
///        (num_seconds * 4)
ResultNode Parser::parse_expr_grouping() {
    (void) eat(TK_LPAREN);
    ResultNode expr =  parse_expr();
    (void) eat(TK_RPAREN);
    return expr;
}


/// @brief Parse an expression being passed to a procedure
ResultNode Parser::parse_proc_argument() {
    return parse_expr();
}


/// @brief Parse an identifier expression. This could
///        be either a function call [foo(...)]
///        or a variable reference [num_seconds]
ResultNode Parser::parse_expr_identifier() {
    token identifier = m_current_token;
    (void) eat(TK_IDENT);

    // See if we have a function call
    if (m_current_token.kind == TK_LPAREN) {
        (void) eat(TK_LPAREN);
        ExpressionProcedureCallNode* call_expr = new ExpressionProcedureCallNode();
        while (m_current_token.kind != TK_RPAREN) {
            ResultNode r_arg_expr = parse_proc_argument();
            if (r_arg_expr.is_err()) {
                std::printf("COULD NOT PARSE ARG\n");
                error_msgs.push_back(
                    VError::create_new(error_type::PARSER_ERR, "Parser::parse_expr_identifier: unable to parse procedure argument")
                );
            }
            ExpressionNode* arg_expr = static_cast<ExpressionNode*>(r_arg_expr.unwrap_or(new ExpressionNode()));
            //call_expr->arguments.push_back(arg_expr);
            call_expr->add_argument(arg_expr);

            if (m_current_token.kind == TK_COMMA) {
                (void) eat(TK_COMMA);
            } else if (m_current_token.kind != TK_RPAREN) {
                // If we do not read a comma, we need to read a ')'
                // If we do not get that either, then this is an error
                error_msgs.push_back(
                    VError::create_new(
                        error_type::PARSER_ERR, 
                        "Expected to get ')' but got {}! when parsing procedure arguments!", 
                        m_current_token.name
                    )
                );
            }
        }

        (void) eat(TK_RPAREN);
        call_expr->set_identifier(identifier);
        // call_expr->identifier = identifier;
        return result::Ok(call_expr);
    } else if (m_current_token.kind == TK_LBRACKET) {
        // Dimension access
        // ident[expr]
        (void) eat(TK_LBRACKET);
        ResultNode r_expr = parse_expr();
        (void) eat(TK_RBRACKET);

        if (r_expr.is_err()) {
            error_msgs.push_back(
                VError::create_new(
                    error_type::PARSER_ERR,
                    "Parser::parse_identifier: unable to parse expression for [] access!"
                )
            );
        }
        
        ExpressionNode* expr = static_cast<ExpressionNode*>(r_expr.unwrap_or(new ExpressionNode()));
        ExpressionIdentifierNode* ident_expr = new ExpressionIdentifierNode();
        // ident_expr->expr = expr;
        ident_expr->set_expr(expr);
        ident_expr->set_identifier(identifier);
        // ident_expr->identifier = identifier;
        return result::Ok(ident_expr);
    } else if (m_current_token.kind == TK_DOT) {
        // Member access
        // test_struct.field;
        // test_struct.method();
        (void) eat(TK_DOT);
        ResultNode r_access_expr = parse_expr_identifier();
        if (r_access_expr.is_err()) {
            error_msgs.push_back(
                VError::create_new(
                    error_type::PARSER_ERR,
                    "Parser::parse_expr_identifier: error parsing member access"
                )
            );
        }
        ExpressionNode* access_expr = static_cast<ExpressionNode*>(r_access_expr.unwrap_or(new ExpressionNode()));
        ExpressionMemberAccessNode* member_expr = new ExpressionMemberAccessNode();
        member_expr->set_identifier(identifier);
        member_expr->set_access(access_expr);

        return result::Ok(member_expr);
    }

    // If not procedure call, then normal variable reference
    ExpressionIdentifierNode* ident_expr = new ExpressionIdentifierNode();
    ident_expr->set_identifier(identifier);
    ident_expr->set_expr(nullptr);
    
    return result::Ok(ident_expr);
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


/// @brief Parse an expression
ResultNode Parser::parse_expr() {
    ResultNode r_lhs = parse_expr_primary();
    if (r_lhs.is_err()) {
        error_msgs.push_back(
            VError::create_new(
                error_type::PARSER_ERR,
                "Parser::parse_expr: Expected expression!"
            )
        );
    }
    ExpressionNode* lhs = static_cast<ExpressionNode*>(r_lhs.unwrap_or(new ExpressionNode()));
  
    /* See if we are at an infix (binary) operator.
     * If so, parse a binary expression */
    while (get_operator_precedence(m_current_token) != precedence::INVALID_OP) {
        // std::printf("GOT OP\n");
        r_lhs = parse_expr_binary(lhs, get_operator_precedence(m_current_token));
        if (r_lhs.is_err()) {
            error_msgs.push_back(
                VError::create_new(error_type::PARSER_ERR, "Parser::parse_expr: Unable to parse rhs of binary expression!")
            );
        }
        lhs = static_cast<ExpressionNode*>(r_lhs.unwrap_or(new ExpressionNode()));
    }

    return result::Ok(lhs);
}


// @brief Parse expressions according to operator precedence
// @param lhs The left-hand-side of the binary expression
// @param min_prec The minimum precedence (binding) for the lhs expression
//                 Basically, if we find an operator that "binds" tighter
//                 we then parse that binary expression first and so on
ResultNode Parser::parse_expr_binary(ExpressionNode* lhs, prec_e min_prec) {
    token op = m_current_token;
    // prec_e op_prec = get_operator_precedence(op);
    (void) eat(); // eat the operator

    ResultNode r_rhs = parse_expr_primary();
    if (r_rhs.is_err()) {
        error_msgs.push_back(
            VError::create_new(error_type::PARSER_ERR, "Parser::parse_expr_binary: unable to parse RHS!")
        );
    }
    ExpressionNode* rhs = static_cast<ExpressionNode*>(r_rhs.unwrap_or(new ExpressionNode()));

    prec_e next_prec = get_operator_precedence(m_current_token);
    if (next_prec > min_prec) {
        r_rhs = parse_expr_binary(rhs, next_prec);
        
        if (r_rhs.is_err()) {
            error_msgs.push_back(
                VError::create_new(error_type::PARSER_ERR, "Parser::parse_expr_binary: unable to parse RHS!")
            );
        }
        rhs = static_cast<ExpressionNode*>(r_rhs.unwrap_or(new ExpressionNode()));
    }

    ExpressionBinaryNode* expr = new ExpressionBinaryNode();
    expr->set_lhs(lhs);
    expr->set_operator(op);
    expr->set_rhs(rhs);

    return result::Ok(expr);
}


/// @brief Parse an expression with a prefix operator
/// !x
/// ~x
ResultNode Parser::parse_expr_prefix() {
    ExpressionPrefixNode* expr = new ExpressionPrefixNode();
    token prefix = m_current_token;
    if (!is_prefix_op(prefix)) {
        return result::Err(VError::create_new(error_type::PARSER_ERR, "Invalid prefix operator {}. Did you mean ! or ~?", token::kind_to_str(prefix.kind)));
    }
  
    expr->set_operator(prefix);
    (void) eat(prefix.kind).unwrap();
    
    ResultNode r_RHS = parse_expr_primary();
    expr->set_rhs(r_RHS);

    return result::Ok(expr);
}


/// @brief Parse the definition of a struct
/// struct Ident
ResultNode Parser::parse_struct() {
    StructDefinitionNode* struct_node = new StructDefinitionNode();
    (void) eat(TK_STRUCT);
    token identifier = m_current_token;
    struct_node->set_identifier(identifier);
    (void) eat(TK_IDENT);

    // Read the definition body
    (void) eat(TK_LSQUIRLY);
    while (m_current_token.kind != TK_RSQUIRLY) {
        ResultNode r_field = parse_struct_member();
        if (r_field.is_err()) {
            error_msgs.push_back(
                VError::create_new(
                    error_type::PARSER_ERR,
                    "Parser::parse_struct: unable to parse member field or method!"
                )
            );
        }
        ASTNode* field = r_field.unwrap_or(new ASTNode());
        struct_node->add_field(field);
    }

    (void) eat(TK_RSQUIRLY);
    return result::Ok(struct_node);
}


ResultNode Parser::parse_struct_member() {
    switch (m_current_token.kind) {
        case TK_IDENT:
            {
                StructMemberFieldNode* field_node = new StructMemberFieldNode();
                
                token identifier = m_current_token;
                (void) eat(TK_IDENT);
                field_node->set_identifier(identifier);
                
                (void) eat(TK_DOUBLECOLON);
                
                ResultNode r_data_type = parse_data_type();
                if (r_data_type.is_err()) {
                    error_msgs.push_back(
                        VError::create_new(
                            error_type::PARSER_ERR,
                            "Parser::parse_struct_member: unable to parse field data type"
                        )
                    );
                }
                ASTNode* data_type = r_data_type.unwrap_or(new ASTNode());
                field_node->set_type_spec(data_type);
                // field_node->type_spec = data_type;

                (void) eat(TK_SEMICOLON);
                
                return result::Ok(field_node);
            }
            break;
        case TK_DEFINE: {
            ResultNode r_method_node = parse_procedure();
            if (r_method_node.is_err()) {
                error_msgs.push_back(
                    VError::create_new(
                        error_type::PARSER_ERR,
                        "Parser::parse_struct_member: unable to parse struct method!"
                    )
                );
            }
            ASTNode* method_node = r_method_node.unwrap_or(new ASTNode());
            return result::Ok(method_node);
        } break;
        default:
            return result::Err(VError::create_new(error_type::PARSER_ERR, "Parser::parse_struct_member: invalid token {}. Exected either identifier or 'proc'.", m_current_token.name));
    }
}

// @brief Parse a variable definition. 
// let x: i32 = 4 * 2;
// let y: i32 = x;
ResultNode Parser::parse_let_statement() {
    (void) eat(TK_LET);
    
    // Get the variable name
    token id_tok = m_current_token;
    auto variable_ident_res = eat(TK_IDENT);
    if (variable_ident_res.is_err()) {
        error_msgs.push_back(variable_ident_res.unwrap_err());
    }
    auto variable_ident_tok = variable_ident_res.unwrap_or(token::create_new(TK_IDENT, "__%internal_ident_err", m_current_token.line_num));

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

    return result::Ok(new VariableDeclarationNode(
        id_tok.name,
        typespec_node,
        expr_node
    ));
}


/// @brief Parse a procedure (function)
/// Return the AST node created from it
///
/// proc ident(ident: type [, ident: type]*) {...}
ResultNode Parser::parse_procedure() {
    ProcedureNode *proc_node = new ProcedureNode();
    
    // Get the 'proc' token 
    token proc_token = m_current_token;
    auto r_proc =  eat(TK_DEFINE);
    if (r_proc.is_err()) {
        auto proc_err = r_proc.unwrap_err();
        error_msgs.push_back(proc_err);
    }
    token proc_tok = r_proc.unwrap_or(
        token::create_new(TK_DEFINE, "__%internal_proc_err", m_current_token.line_num)
    );
   
    // Eat the identifier
    token ident_tok = m_current_token;
    auto identifier_res = eat(TK_IDENT);
    if (identifier_res.is_err()) {
        auto identifier_err = identifier_res.unwrap_err();
        error_msgs.push_back(identifier_err);
    }
    proc_node->set_name(ident_tok.name);

    (void) eat(TK_LPAREN).unwrap_or(token::create_new(TK_LPAREN, "__%internal_lparen_err", m_current_token.line_num));
    
    // Parse the procedure parameters
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
        
        (void) eat(TK_COMMA).unwrap();
    }
   
    (void) eat(TK_RPAREN).unwrap_or(token::create_new(TK_RPAREN, "__%internal_rparen_err", m_current_token.line_num));
    (void) eat(TK_COLON).unwrap_or(token::create_new(TK_COLON, "__%internal_colon_err", m_current_token.line_num));

    // Get the return type specification
    auto return_type = parse_data_type().unwrap();
    proc_node->set_return_type(return_type);

    // Parse the code body
    ResultNode r_body = parse_scope();
    if (r_body.is_err()) {
        error_msgs.push_back(r_body.unwrap_err());
    }
    CodeBlockStatementNode* body = 
        static_cast<CodeBlockStatementNode*>(r_body.unwrap_or(new CodeBlockStatementNode()));
    proc_node->set_body(body);
    
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
//    while (m_current_token.kind != TK_EOF) {
//        switch (m_current_token.kind) {
//            case TK_DEFINE: {
//                auto node = parse_procedure().unwrap();
//                m_ast->add_node(node);
//            } break;
//            case TK_LET: {
//                auto node = parse_let_statement().unwrap();
//                (void) eat(TK_SEMICOLON);
//                m_ast->add_node(node);
//            } break;
//            case TK_STRUCT: {
//                auto node = parse_struct().unwrap();
//                m_ast->add_node(node);
//            } break;
//            default:
//                std::printf("DEFAULT TOKEN: %s\n", token::kind_to_str(m_current_token.kind).c_str());
//                break;
//        }
//    }
    auto node = parse_top_level_statement();
    while (node.has_value()
           && node.value()->kind != AST_INVALID_NODE
    ) {
        node = parse_top_level_statement();
    }
    
    return m_ast;
}

std::optional<ASTNode*> Parser::parse_top_level_statement() {
    if (m_current_token.kind == TK_EOF) {
        return std::nullopt;
    }


    switch (m_current_token.kind) {
        case TK_DEFINE: {
            auto node = parse_procedure().unwrap();
            m_ast->add_node(node);
            return node;
        } break;
        case TK_LET: {
            auto node = parse_let_statement().unwrap();
            (void) eat(TK_SEMICOLON);
            m_ast->add_node(node);
            return node;
        } break;
        case TK_STRUCT: {
            auto node = parse_struct().unwrap();
            m_ast->add_node(node);
            return node;
        } break;
        default: {
            ASTNode* node = new ASTNode(AST_INVALID_NODE);
            return node;
        } break;
    }
}


/// @brief Look at the next token in the stream without advancing the current token
token Parser::peek_token() {
    // return m_lexer->peek_token();
    return m_peek_token;
}


/// @brief Return the current token
token Parser::current_token() {
    return m_current_token;
    // return token_queue[0];
}

/// @brief Eat the expected token
token Parser::eat() {
//    std::printf(
//        "current: %s. peek: %s\n",
//        token::kind_to_str(m_current_token.kind).c_str(),
//        token::kind_to_str(m_peek_token.kind).c_str()
//    );

    // m_current_token = m_lexer->next_token();
    m_current_token = m_peek_token;
    m_peek_token = m_lexer->next_token();
    return m_current_token;
}

/// @brief Eat the expected token
result::Result<token, VError> Parser::eat(token_kind type) {
    if (m_current_token.kind != type) {
        std::printf("Parser::eat(type): meant to read [%s] but got [%s]!\n"
                ,token::kind_to_str(type).c_str()
                ,token::kind_to_str(m_current_token.kind).c_str()
        );
        return result::Err(VError::create_new(
            error_type::PARSER_ERR,
            "Parser::eat: Expected {} but got {}\n",
            token::kind_to_str(type).c_str(),
            token::kind_to_str(m_current_token.kind).c_str()
        ));
    }
   
//    std::printf(
//        "current: %s. peek: %s\n",
//        token::kind_to_str(m_current_token.kind).c_str(),
//        token::kind_to_str(m_peek_token.kind).c_str()
//    );
    // m_current_token = m_lexer->next_token();
    m_current_token = m_peek_token;
    m_peek_token = m_lexer->next_token();
    return result::Ok(m_current_token);
}

}
