#include <stdexcept>
#include <tests/preprocessor.hh>

#include <cstdio>

int main(int argc, char** argv) {
    printf("TESTING PREPROCESSOR\n");

    if (argc < 2)
        throw std::runtime_error("usage: bin/preprocessor <file path>");

    test_preprocessor(argv[1]);
    return 0;
}
