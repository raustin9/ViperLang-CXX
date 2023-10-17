#pragma once
#include "defines.hh"
/*
 * Preprocessor
 * The purpose of the preprocessor is to execute certain bits of functionality at compile time 
 * instead of at runtime. Common examples of this would be macros and comments.
 *
 * This is done by performing its own passes over the source code before handing 
 * it off to the next phase (typically the lexer).
 * 
 * The input of the preprocessor is source code, and the output is source code. This means that 
 * the transformation that it is performing are going to be writing and editing the source code
 * that it is given. In the case of comments, it is removing them. In the case of macros, it is 
 * inserting that into the source code.
 */

#include <string>

class Preprocessor {
    public:
        Preprocessor(std::string source_code);
        ~Preprocessor();
        Preprocessor(const Preprocessor&) = delete;
        Preprocessor& operator= (const Preprocessor&) = delete;

        // Accessors
        std::string get_source();       // get the original code that was input
        std::string get_transformed();  // get the transformed source code after processing

        // Preprocessor API functions
        void process_comments();        // process the comments by removing them from the source code
        void process_macros();          // process macros by inserting them properly into the source code
        void process();

    private:
        std::string _code;
        std::string _transformed_code;
        char _cur_char;
        int _position;
        int _read_position;

        void _advance_char();
        char _peek_char();
        void _single_line_comment();
        void _multi_line_comment();
};
