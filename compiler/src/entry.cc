#include "entry.hh"

#include <iostream>

std::string
test_preprocessor(char* source_name) {
    std::string source_code = read_file(source_name);

    Preprocessor preprocessor = Preprocessor(source_code);
    std::cout << "Original source code:" << std::endl << preprocessor.get_source();
    preprocessor.process_comments();
    
    return preprocessor.get_transformed();
}
