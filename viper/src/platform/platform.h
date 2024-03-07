/*
 * Handles platform-specific tasks.
 */

#pragma once

#include "defines.h"
#include <string>

namespace platform {

/// @brief Print a line to the screen
void print_line(const std::string& text, u8 color);

void print_error(const std::string& text, u8 color);

}
