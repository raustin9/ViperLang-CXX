#pragma once

#include <cstdio>
#include <format>
#include <string>
#include <string_view>

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
        template <typename ... Args>
        static VError create_new(error_type type, const std::string_view& fmt, Args... args) {
            VError err = VError();

            // char buffer[400];
            err.msg = std::vformat(fmt, std::make_format_args(args...));
            // std::snprintf(buffer, sizeof(buffer), fmt, args...);
            
            // err.msg = buffer;
            return err;
        }

    private:
        VError() {}

        std::string msg;
};

}
