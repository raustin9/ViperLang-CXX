#include "defines.h"
#include "platform.h"
#include <cstdio>
#include <cstring>
#include <unordered_map>
#include <stdarg.h>

#ifdef Q_PLATFORM_LINUX

namespace platform {

/// Print a line to the console
void print_line(color_e color, const char* fmt, ...) {
    std::unordered_map<color_e, const char*> colormap;
    colormap[WHITE] = "\x1b[37m";
    colormap[RED] = "\x1b[31m";
    colormap[GREEN] = "\x1b[32m";
    colormap[BLUE] = "\x1b[34m";
    colormap[CYAN] = "\x1b[36m";
    colormap[YELLOW] = "\x1b[33m";
    colormap[MAGENTA] = "\x1b[35m";

    const char* color_prefix = colormap[color];

    constexpr u32 msg_len = 400;
    char msg[msg_len];
    std::memset(msg, 0, sizeof(msg));

    // Format the message
    __builtin_va_list arg_ptr;
    va_start(arg_ptr, fmt);
    vsnprintf(msg, msg_len, fmt, arg_ptr);
    va_end(arg_ptr);

    std::fprintf(stdout, "%s%s\x1b[0m", color_prefix, msg);
}


/// Print an error line to the console
void print_error(const char* fmt, ...) {
    constexpr u32 msg_len = 400;
    char msg[msg_len];
    std::memset(msg, 0, sizeof(msg));

    // Format the message
    __builtin_va_list arg_ptr;
    va_start(arg_ptr, fmt);
    vsnprintf(msg, msg_len, fmt, arg_ptr);
    va_end(arg_ptr);
    
    std::fprintf(stderr, "\x1b[31m%s\n\x1b[0m", fmt);
}

}

#endif // Q_PLATFORM_LINUX
