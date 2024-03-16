#include "preprocessor.h"
#include "core/viper.h"
#include "defines.h"
#include "token.h"

#include <format>

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
    pp.m_parent_file = nullptr;

    return pp;
}

/// @brief Create a new preprocessor for a module
/// @param input_tokens The list of tokens we are preprocessing
Preprocessor Preprocessor::create_new(VFile* parent, const std::list<token>& input_tokens) {
// Preprocessor Preprocessor::create_new(const std::vector<token>& input_tokens) {
    Preprocessor pp = Preprocessor();

    pp.m_tokens = input_tokens;
    pp.m_current_position = m_tokens.begin();
    // pp.m_current_position = 0;
    pp.m_current_token = token::create_new(TK_ILLEGAL, "", 0);
    pp.m_peek_token = token::create_new(TK_ILLEGAL, "", 0);
    pp.m_parent_file = parent;

    return pp;
}


/// @brief Eat the current token and advance to the next one
void Preprocessor::_next_token() {
    m_current_token = m_peek_token;

    if (m_current_position != m_tokens.end()) {
        m_peek_token = *std::next(m_current_position, 1);
    }

    m_current_position++;
}


/// @brief Handle importing a module
std::optional<VError> Preprocessor::_handle_import() {
    _next_token(); // Eat the #import token
                   
    if (m_current_token.kind == TK_LBRACKET) {
        // Local module import
    } else if (m_current_token.kind == TK_LSQUIRLY) {
        // Standard lib import
    } else {
        return VError::create_new(std::format("Expected '{' or '['. Got {}", m_current_token.name), ERR_LEVEL_FATAL);
    }



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
            case TK_PP_IMPORT:
                _handle_import();
            default:
                break;
        }
    }

    return m_tokens;
}



}
