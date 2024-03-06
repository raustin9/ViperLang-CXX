#include <io>
#include <string>

struct User {
    name :: std::string;
    age :: u32;
    password :: std::string;
}

proc test_function() : i32 {
    // Do something...
    return 0;
}

proc main() : i32 {
    let alex: User = {};

    alex.name = std::string("Alex"); // No implicit constructors
    alex.age = 22;
    alex.password = std::string("****");

    let basic_string: cstr = "Alex"; // cstr is string literal type

    void = test_function();

    return 0;
}
