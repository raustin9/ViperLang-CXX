#include <cstdlib>
#include <iostream>
#include <entry.hh>
#include <stdexcept>

int 
main(int argc, char** argv) {
    if (argc < 2) {
        throw std::runtime_error("usage: hero <file name>");
    }


    std::string pre_test = test_preprocessor(argv[1]);
    std::cout << std::endl << "Post preprocessor:" << std::endl << pre_test << std::endl;
    
    return 0;
}
