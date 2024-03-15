#pragma once

#include <string>

namespace viper {

/* Severity of compiler error */
enum error_level {
    ERR_LEVEL_WARNING, // Recoverable or unreccomended
    ERR_LEVEL_FATAL,   // Completely invalid, unrecoverable
};

/* Error class for something we ran into when compiling */
class VError {
    public:
        ~VError() {}
        static VError create_new(const std::string& message, error_level level);
        void print();
    private:
        VError() {}

        std::string m_msg;
        error_level m_level;
};

}
