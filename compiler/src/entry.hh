#include "defines.hh"
#include "preprocessor/preprocessor.hh"
#include "core/file.hh"
#include <cstdio>
#include <cstdlib>
#include <iostream>

HAPI void hello_world();

int
main(int argc, char** argv) {
    if (argc < 2) {
        printf("usage: hero <program path>");
        exit(EXIT_FAILURE);
    }

    std::string source_code = read_file(argv[1]);

    Preprocessor preprocessor = Preprocessor(source_code);
    std::cout << "Original source code:" << std::endl << preprocessor.get_source();
    preprocessor.process_comments();
    
    std::cout << "Transformed source code:" << std::endl << preprocessor.get_transformed();
}
