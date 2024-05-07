#pragma once
#include "core/core.h"
#include "defines.h"
#include <algorithm>
#include <stdexcept>

namespace viper {

struct CodeLocation {
    u64 line_number;
    u64 column_number;

    /// Find which codelocation was found first in the source code file.
    /// This assumes both of these locations are from the same file
    static const CodeLocation& Min(const CodeLocation& u, const CodeLocation& v) {
        // If they share the same line, then which appears first
        if (v.line_number == u.line_number) {
            if (v.column_number < u.column_number) {
                return v;
            } 
            return u;
        }

        if (v.line_number < u.line_number) {
            return v;
        } 
        return u;
    }
    
    /// Find which codelocation was found later in the source code file.
    /// This assumes both of these locations are from the same file
    static const CodeLocation& Max(const CodeLocation& u, const CodeLocation& v) {
        // If they share the same line, then which appears first
        if (v.line_number == u.line_number) {
            if (v.column_number > u.column_number) {
                return v;
            } 
            return u;
        }

        if (v.line_number > u.line_number) {
            return v;
        } 
        return u;
    }
}; 

/// Defines a region inside the source code that something appears in
class Span {
public:
    Span(VFile* file, CodeLocation start, CodeLocation end) : file(file), start(start), end(end) {}

    /// Add two `Span`s together
    /// This will form a region contained by both of them
    Span operator+(const Span& other) const {
        if (this->file != other.file) {
            throw std::runtime_error("Cannot add two Spans that are not in the same file!");
            return *this;
        }

        return Span(
            this->file,
            CodeLocation::Min(this->start, other.start),
            CodeLocation::Max(this->end, other.end)
        );
    }
   
private:
    VFile* file;
    CodeLocation start;
    CodeLocation end;

};

} // viper namespace
