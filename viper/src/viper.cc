#include "viper.h"
#include "tokenizer/tokenizer.h"

int main(int argc, char** argv) {
    core::VFile file = core::VFile::from("test.viper");

    std::vector<viper::token> tokens = viper::tokenizer::tokenize_file(&file);

    std::printf("Input: \n%s\n", file.content.c_str());
    for (const auto& tok : tokens) {
        tok.print_name();
    }

    return EXIT_SUCCESS;
}
