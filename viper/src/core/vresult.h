#pragma once

#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>

namespace viper {

/* Severity of compiler error */
enum error_level {
    ERR_LEVEL_WARNING, // Recoverable or unreccomended
    ERR_LEVEL_FATAL,   // Completely invalid, unrecoverable
};

namespace types {
    template <typename T>
        struct Ok {
            Ok(const T& val) : val(val) {}
            Ok(T&& val) : val(std::move(val)) {}

            T val;
        };

    template<>
    struct Ok<void> {};

    template <typename E>
    struct Err {
        Err(const E& val) : val(val) {}
        Err(E&& val) : val(std::move(val)) {}
        E val;
    };
}

template <typename T, typename CleanT = typename std::decay<T>::type>
types::Ok<CleanT> Ok(T&& val) {
    return types::Ok<CleanT>(std::forward<T>(val));
}

inline types::Ok<void> Ok() {
    return types::Ok<void>();
}

template <typename E, typename CleanE = typename std::decay<E>::type>
types::Err<CleanE> Err(E&& val) {
    return types::Err<CleanE>(std::forward<E>(val));
}

/* Error class for something we ran into when compiling */
template <typename T, typename E>
struct VResult {
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

    T unwrap() {
        if (ok) {
            return value;
        } 

        throw std::runtime_error("Result type is not OK");
    }

    std::variant<T, E> value;

    private:
        bool ok;
};


}
