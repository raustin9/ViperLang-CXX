#import <string>
#import <memory>
#import [localfile.viper]

struct Person {
    name :: std::string;
    age :: u32;
}

proc test_procedure(param1: i32, param2: f32): bool {
    return true;
}

proc main(): i32 {
    let x: i32  = 0;
    let y: f32 = 0.0;
    void = test_procedure(x, y);

    let p: Person = std::allocate(Person);
    defer std::deallocate(p);

    return 0;
}
