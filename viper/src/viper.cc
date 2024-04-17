#include <iostream>
#include "core/result.h"

viper::result::Result<std::string, int> test() {
    return viper::result::Err(1);
;
}

int main(int argc, char** argv) {
    std::printf("VIPER\n");

    std::cout << "Result: " << test().unwrap() << "\n";

    return EXIT_SUCCESS;
}
