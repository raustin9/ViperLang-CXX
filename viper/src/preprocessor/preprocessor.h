#pragma once

#include "core/viper.h"
#include "defines.h"
#include "token.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <optional>

namespace viper {

enum DIRECTIVE {
    INCLUDE,
    DEFINE,
    INVALID
};

/// TODO: Macros
/// -- undecided on language spec for metaprogramming
struct macro {
    std::string name; // identifier for the macro
    
    data_type_primitive type;

    bool is_proc;     // whether it is a proc macro or declaritive
    std::vector<std::string> params;
    
};

class Preprocessor {
    public:
        ~Preprocessor() {}
        static Preprocessor create_new(const std::vector<token>& input_tokens);
        static Preprocessor create_new(VFile* parent, const std::vector<token>& input_tokens);

        std::vector<token> process(); // Preprocess tokens and 

    private:
        Preprocessor() {}

        void _next_token();
        std::optional<VError> _handle_import();

        VFile* m_parent_file;
        std::unordered_map<std::string, macro> m_macros;
        std::vector<token> m_tokens;
        token m_current_token;
        token m_peek_token;
        u64 m_current_position;
};

}
