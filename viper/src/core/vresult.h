#pragma once

#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>

namespace viper {

namespace vresult {

// // Unit type
// struct unit_t {}; 
// 
// template <typename T>
// struct Ok {
//     using value_type = T;
// 
//     Ok(const T& val) : val(val) {}
//     Ok(T&& val) : val(std::move(val)) {}
// 
//     T val;
// };
// 
// template <>
// struct Ok<unit_t> {
//     using value_type = unit_t;
// 
//     Ok() = default;
//     Ok(unit_t) {}
// 
//     unit_t val;
// };
// 
// template <typename E>
// struct Err {
//     Err(const E& val) : val(val) {}
//     Err(E&& val) : val(std::move(val)) {}
//     E val;
// };
// 
// Ok()->Ok<unit_t>; // ???

/* Error class for something we ran into when compiling */
template <typename T, typename E>
struct VResult {
    static_assert(!std::is_same<E, void>::value, "void error type is not permitted");

    ~VResult() {}
    void print();

    static VResult<T,E> Ok(T val) {
        VResult<T, E> res;
        res.ok = true;

        res.value = val;

        return res;
    }
    
    static VResult<T,E> Err(E) {

    }
    
    bool is_ok() {
        return ok == true;
    }

    bool is_err() {
        return ok == false;
    }

    /// @brief Return the value of the result. This throws a runtime exception if the result is an Error
    T unwrap() {
        if (ok) {
            return value;
        }

        throw std::runtime_error("Result type is not OK");
    }

    /// @brief Returns the value of the result if it is valid.
    /// If the result is an error, then it terminates the program
    T expect(const char* msg) const {
        if (!ok) {
            std::fprintf(stderr, "%s\n", msg);
            std::terminate();
        }

        return value;
    }


    private:
        bool ok;                  // Flag for if this is Ok or Err. true for Ok, false for Err
        std::variant<T, E> value; // The actual value being held by the result
};

} // namespace vresult
} // namespace viper
