#include <tests/lexer.hh>

#include <cstdio>

int main(int argc, char** argv) {
    printf("TESTING LEXER\n");

    if (argc < 2)
        throw std::runtime_error("usage: bin/preprocessor <file path>");

    std::vector<token_t> lex_test = test_lexer(argv[1]);
    for (size_t i = 0; i < lex_test.size(); i++) {
        print_token(lex_test[i]);
    }
    return 0;
}
