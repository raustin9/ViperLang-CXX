// How to create a module

module TestMod {

struct TestStruct {
    field1 :: i32;
    field2 :: f32;
}

proc test(): i32 {
    // Does something
    return 0;
}

}

// In another file you want to use this module in...
#include [TestMod]
proc main(): i32 {
    let t: TestMod::TestStruct = {};

    void = TestMod::test();

    return 0;
}

} // TestMod
