#include "preprocessor.h"
#include "core/error.h"
#include "defines.h"
#include "token.h"
#include <vector>

namespace viper {


/// @brief Create a new preprocessor for a module
/// @param input_tokens The list of tokens we are preprocessing
Preprocessor Preprocessor::create_new(const std::list<token>& input_tokens) {
// Preprocessor Preprocessor::create_new(const std::vector<token>& input_tokens) {
    Preprocessor pp = Preprocessor();

    pp.m_tokens = input_tokens;
    pp.m_current_position = m_tokens.begin();
    // pp.m_current_position = 0;
    pp.m_current_token = token::create_new(TK_ILLEGAL, "", 0);
    pp.m_peek_token = token::create_new(TK_ILLEGAL, "", 0);

    return pp;
}



/// @brief Eat the current token and advance to the next one
void Preprocessor::_next_token() {
    m_current_token = m_peek_token;

    if (m_current_position != m_tokens.end()) {
        m_peek_token = *std::next(m_current_position, 1);
    }

//     if (m_current_position < m_tokens.size()) {
//         m_peek_token = 
//         // m_peek_token = m_tokens.;
//     }

    m_current_position++;
}


/// @brief See if we are trying to export something valid
std::optional<VError> Preprocessor::_handle_export() {
    _next_token();

    if (m_current_token.kind != TK_PROC) {
        return VError::create_new("Expected \"proc\"", ERR_LEVEL_FATAL);
    }

    // TODO: 

    return std::nullopt;
}



/// @brief Take list of tokens and perform preprocessing transformations on them
std::list<token> Preprocessor::process() {
// std::vector<token> Preprocessor::process() {
    if (m_tokens.size() == 0) {
        std::printf("Preprocessor::process: Token input is empty\n");
    }

    _next_token();
    _next_token();

    while (m_current_token.kind != TK_EOF) {
        switch(m_current_token.kind) {
            
            default:
                break;
        }
    }

    return m_tokens;
}



}
