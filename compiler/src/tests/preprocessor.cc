#include "preprocessor.hh"

// Test the output of the preprocessor
// This outputs the transformed source code after the preprocessor has done its passes
// To test this, compare the output against a string or file with the expected output
std::string
test_preprocessor(char* source_name) {
    std::string source_code = read_file(source_name);

    Preprocessor preprocessor = Preprocessor(source_code);
    std::cout << "Original source code:" << std::endl << preprocessor.get_source();
    preprocessor.process_comments();
    
    return preprocessor.get_transformed();
}
