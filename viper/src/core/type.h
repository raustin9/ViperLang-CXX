#pragma once

#include "defines.h"

namespace viper {

struct Type {
    enum Kind {
        PLACEHOLDER   = 0,
        VOID          = 1,
        BOOL          = 2,
        INT           = 3,
        FLOAT         = 4,
        CHAR          = 5,
        NONE          = 6,
        POINTER       = 7,
        REF           = 8,
        ARRAY         = 9,
        SLICE         = 10,
        DYNAMIC_ARRAY = 11,
        STRUCT        = 12,
        ENUM          = 13,
        SUM           = 14,
        TUPLE         = 15,
        PROCEDURE     = 16
    };

    enum Sign {
        UNSIGNED = 0,
        SIGNED   = 1
    };

    Kind kind;

    bool is_primative() const {
        return kind == VOID 
            || kind == BOOL 
            || kind == INT 
            || kind == FLOAT 
            || kind == CHAR 
            || kind == NONE;
    }
};

// TODO: PlaceHolder types? 

struct IntType : public Type {
    Sign sign;
    u64 width;

//    static const Type* get(Sign sign, u64 width) {
//        IntType itype = IntType();
//        itype.sign = sign;
//        itype.width = width;
//    }
};


} // viper namespace
