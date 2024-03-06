#include <iostream>
#include "viper.h"
#include "tokenizer/tokenizer.h"

int main(int argc, char** argv) {
    std::string input = "proc main(): i32 {}";
    viper::VFile file;
    file.name = "test.viper";
    file.content = input;

    std::vector<viper::token> tokens = viper::tokenizer::tokenize_file(&file);

    std::printf("Input: %s\n", file.content.c_str());
    std::printf("%lu tokens\n", tokens.size());
    for (const auto& tok : tokens) {
        tok.print_name();
    }

    return EXIT_SUCCESS;
}
