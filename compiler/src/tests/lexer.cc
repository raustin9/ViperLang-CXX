#include "lexer.hh"

std::vector<token_t>
test_lexer(char* source_name) {
    std::string source_code = read_file(source_name);

    Preprocessor preprocessor = Preprocessor(source_code);
    std::cout << "Original source code:" << std::endl << preprocessor.get_source();
    preprocessor.process_comments();
    std::cout << "\nTransformed source code:" << std::endl << preprocessor.get_transformed() << std::endl;
    std::string lexinput = preprocessor.get_transformed();

    Lexer lexer = Lexer(lexinput);
    lexer.tokenize_input();

    return lexer.get_token_stream();
}
