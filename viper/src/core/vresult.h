#pragma once

#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>

namespace viper {

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
    
    static VResult<T,E> Err(E val) {
        VResult<T, E> res;
        res.ok = true;

        res.value = val;

        return res;
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
            return std::get<T>(value);
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

        return std::get<T>(value);
    }

    /// @brief Return the value of the result or a specified value if the result is an error
    /// Instead of terminating when it's Err, we return the val parameter
    T&& unwrap_or(T&& val) {
        if (!ok) {
            return std::move(val);
        }

        return std::move(std::get<T>(value));
    }

    T& unwrap_or(T& val) {
        if (!ok) {
            return val;
        }

        return std::get<T>(value);
    }


    private:
        bool ok;                  // Flag for if this is Ok or Err. true for Ok, false for Err
        std::variant<T, E> value; // The actual value being held by the result
};

} // namespace viper
