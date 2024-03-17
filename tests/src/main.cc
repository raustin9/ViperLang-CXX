#include "test_manager.h"
#include "tokenizer/tokenizer_test.h"
#include "preprocessor/preprocessor_test.h"

int main(void) {
    TestManager manager = TestManager();

    tokenizer_register_tests(manager);
    preprocessor_register_tests(manager);

    manager.run_tests();
    return 0;
}
