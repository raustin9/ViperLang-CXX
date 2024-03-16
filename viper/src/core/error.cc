#include "viper.h"

#include <cstring>

namespace viper {

/// @brief Print the formatted error message
void VError::print() {
    char prefix[10];

    switch(m_level) {
        case ERR_LEVEL_FATAL:
            std::strcpy(prefix, "[FATAL]");
            break;
        case ERR_LEVEL_WARNING:
            std::strcpy(prefix, "[WARNING]");
            break;
    }

    std::printf("%s %s\n", prefix, m_msg.c_str());
}


/// @brief Create a new compiler error object
/// @param message The message for the error
/// @param level The severity of the encountered error
VError VError::create_new(const std::string &message, error_level level) {
    VError verr = VError();

    verr.m_msg = message;
    verr.m_level = level;

    return verr;
}

}
