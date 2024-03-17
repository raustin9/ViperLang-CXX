#include "parser.h"
#include "tokenizer/tokenizer.h"

namespace viper {

/// @brief Create a new parser from a specified Tokenizer
Parser Parser::create_new(Tokenizer* lexer) {
    Parser p = Parser();
    p.m_lexer = lexer;

    return p;
}

/// @brief Parse the tokens and generate the AST
/// @returns the Abstract Syntax tree
std::shared_ptr<AST> Parser::parse() {
    switch (m_current_token.kind) {
        default:
            break;
    }

    return {};
}


/// @brief Eat the expected token
token Parser::eat(token_kind type) {
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
    return m_current_token;
}

}
