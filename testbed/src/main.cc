#include "core/token.hh"
#include <cstdlib>
#include <iostream>
#include <entry.hh>
#include <stdexcept>

int 
main(int argc, char** argv) {
    if (argc < 2) {
        throw std::runtime_error("usage: hero <file name>");
    }


    // std::string pre_test = test_preprocessor(argv[1]);
    // std::cout << std::endl << "Post preprocessor:" << std::endl << pre_test << std::endl;

    std::vector<token_t> lex_test = test_lexer(argv[1]);
    std::cout << "Token stream: " << std::endl;
    for (size_t i = 0; i < lex_test.size(); i++) {
        print_token(lex_test[i]);
    }
    
    return 0;
}
