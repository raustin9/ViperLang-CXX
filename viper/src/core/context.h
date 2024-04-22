#pragma once

#include "core/core.h"
#include "defines.h"

#include <string>

namespace viper {

struct Location {
    i32 line;
    i32 character;
};

struct Context {
    VFile* file;
    VModule* module;
};

}
