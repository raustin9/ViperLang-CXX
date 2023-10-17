#include "token.hh"

token_t 
create_token(std::string literal, token_e token, size_t line_num) {
    token_t tok = {token, literal, line_num};
    return tok;
}

void 
print_token(token_t tok) {
    printf("Literal: '%s' on line %lu\n", tok.literal.c_str(), tok.line_num);
}
