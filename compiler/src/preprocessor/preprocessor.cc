#include "preprocessor/preprocessor.hh"

/*
 * CONSTRUCTOR
 */
Preprocessor::Preprocessor(std::string source_code) {
    _code = source_code;
    _transformed_code = source_code;
    _position = 0;
    _read_position = 0;
}

/*
 * DESTRUCTOR
 */
Preprocessor::~Preprocessor() {
    
}

/*
 * PUBLIC
 */

// Get the original source code of the preprocessor
HAPI std::string
Preprocessor::get_source() {
    return _code;
}

// Get the transformed source code of the preprocessor
std::string
Preprocessor::get_transformed() {
    return _transformed_code;
}

char
Preprocessor::_peek_char() {
    if (_read_position >= _transformed_code.length()) {
        return '\0';
    } else {
        return _transformed_code[_read_position];
    }
}

// Process comments in the source code
// This is done by simply removing them from the source code
void
Preprocessor::process_comments() {
    _advance_char();
    // _advance_char();
    while (_cur_char != '\0') {
        switch(_cur_char) {
            case '/':
                if (_peek_char() == '/') {
                    printf("SINGlE LINE\n");
                    _single_line_comment();
                } else if (_peek_char() == '*') {
                    _multi_line_comment();
                }
                break;
            default:
                break;
        }

        _advance_char();
    }
}

void
Preprocessor::_advance_char() {
    if (_read_position >= _transformed_code.length()) {
        _cur_char = '\0';
    } else {
        _cur_char = _transformed_code[_read_position];
    }

    _position = _read_position;
    _read_position++;
    return;
}

void
Preprocessor::_single_line_comment() {
    while (_cur_char != '\n') {
        _transformed_code[_position] = ' ';
        _advance_char();
    }
}

void
Preprocessor::_multi_line_comment() {
    while (true) {
        if (_cur_char == '*') {
            if (_peek_char() == '/') {
                _transformed_code[_position] = ' ';
                _advance_char();
                _transformed_code[_position] = ' ';
                _advance_char();
                break;
            }
        }

        _transformed_code[_position] = ' ';
        _advance_char();
    }

}


// Process macros in the source code
// This is done by inserting them into the transformed source code
void
Preprocessor::process_macros() {

}

// Perform all preprocessor transformations on the source code
void
Preprocessor::process() {
    this->process_comments();
}
