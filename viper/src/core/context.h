#pragma once

#include "defines.h"

#include <string>

namespace viper {

struct Location {
    i32 line;
    i32 character;
};

struct Context {
    std::string filename;
    Location begin;
    Location end;

    Context() {}
    Context(std::string filename, Location begin) {}

    void start(Context* context);
    void finish(Context* context, Context* wtspccontext = nullptr);
    Context lastchar();
};

}
