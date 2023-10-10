#include "preprocessor/preprocessor.hh"

/*
 * CONSTRUCTOR
 */
Preprocessor::Preprocessor(std::string &source_code) {
    _code = source_code;
    _transformed_code = "";
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

// Process comments in the source code
// This is done by simply removing them from the source code
void
Preprocessor::process_comments() {
    char current;
    for (size_t i = 0; i < _code.length(); i++) {
        current = _code[i];
        switch(current) {
            case '/':
                if (i < _code.length()-1) {
                    if (_code[i+1] == '/') {
                        // Found '//' -- single line comment
                        while (_code[i] != '\n') 
                            i++;
                    } else if (_code[i+1] == '*') {
                        // Found '/*' -- multi line comment
                        while (1) {
                            if (_code[i] == '*' && i < _code.length()-1) {
                                if (_code[i+1] == '/') {
                                    // Found '*/' -- end multi-line comment
                                    i += 2;
                                    break;
                                }
                            } else if (i == _code.length()-1) {
                                break;
                            }
                            i++;
                        }
                    }
                }
                break;
            default:
                break;
        }
        _transformed_code.push_back(_code[i]);
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
