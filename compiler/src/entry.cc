#include "entry.hh"

#include <iostream>

// // Test the output of the preprocessor
// // This outputs the transformed source code after the preprocessor has done its passes
// // To test this, compare the output against a string or file with the expected output
// std::string
// test_preprocessor(char* source_name) {
//     std::string source_code = read_file(source_name);
// 
//     Preprocessor preprocessor = Preprocessor(source_code);
//     std::cout << "Original source code:" << std::endl << preprocessor.get_source();
//     preprocessor.process_comments();
//     
//     return preprocessor.get_transformed();
// }

// Test the output of the lexer
// This outputs the stream of tokens that the lexer produces from the source code that it is given.
// To test if this output is correct, compare this with a file or string to see if the output matches what is expected
//std::vector<token_t>
//test_lexer(char* source_name) {
//    std::string source_code = read_file(source_name);
//
//    Preprocessor preprocessor = Preprocessor(source_code);
//    std::cout << "Original source code:" << std::endl << preprocessor.get_source();
//    preprocessor.process_comments();
//    std::cout << "\nTransformed source code:" << std::endl << preprocessor.get_transformed() << std::endl;
//    std::string lexinput = preprocessor.get_transformed();
//
//    Lexer lexer = Lexer(lexinput);
//    lexer.tokenize_input();
//
//    return lexer.get_token_stream();
//}
