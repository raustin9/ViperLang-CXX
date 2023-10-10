#include "preprocessor/preprocessor.hh"

/*
 * CONSTRUCTOR
 */
Preprocessor::Preprocessor(std::string &source_code) {
    _code = source_code;
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
