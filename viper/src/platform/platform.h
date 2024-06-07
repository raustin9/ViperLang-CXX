/*
 * Handles platform-specific tasks.
 */

#pragma once

#include "defines.h"

namespace platform {

/// The list of colors that we are able to print
enum color_e: u8 {
    WHITE,
    RED,
    BLUE,
    GREEN,
    CYAN,
    MAGENTA,
    YELLOW,
};

/// Print a line to the console
void print_line(color_e color, const char* fmt, ...);

/// Print an error line to the console
void print_error(const char* fmt, ...);

} // Platform namespace
