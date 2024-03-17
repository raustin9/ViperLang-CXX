#include "core/viper.h"
#include "preprocessor_test.h"
#include "tokenizer/tokenizer.h"

#include <cstdint>
#include <preprocessor/preprocessor.h>

uint8_t pp_test_imports() {
    viper::VFile* file = viper::VFile::create_new_ptr();

    file->name = "test.viper";
    file->content = 
        "#import <string>\n"
        "#import [test_module]\n";

    viper::Tokenizer lexer = viper::Tokenizer::create_new(file);
    auto tokens = lexer.tokenize_file();

    viper::Preprocessor pp = viper::Preprocessor::create_new(file, tokens);
    pp.process();

    // for (const auto& [key, value] : file->dependency_modules) {
        // std::printf("%s\n", key.c_str());
    // }

    return true;
}

void preprocessor_register_tests(TestManager& manager) {
    manager.register_test(pp_test_imports, "test if specified imports are valid");
}
