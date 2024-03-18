#pragma once

#include <string>

namespace viper {

enum class error_type {
    LEXER_ERR,
    PREPROCESSOR_ERR,
    PARSER_ERR,
};

class VError {
    public:
        ~VError() {}

        /// @brief Create a new VError and return it
        static VError create_new(error_type type, const std::string& msg) {
            VError err = VError();
            err.msg = msg;

            return err;
        }

    private:
        VError() {}

        std::string msg;
};

}
