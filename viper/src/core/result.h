#pragma once

#include <iostream>
#include <exception>
#include <functional>
#include <optional>
#include <ostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>

namespace viper {

namespace result {

template <typename T, typename E>
class Result;

template <typename T>
struct Ok {
    explicit constexpr Ok(const T& value) : m_value(value) {}
    explicit constexpr Ok(T&& value) : m_value(std::move(value)) {}

    constexpr const T& value() const& { return m_value; }
    constexpr T&& value() && { return std::move(m_value); }

    template <typename E>
    constexpr operator Result<T, E>() const& {
        return Restult<T, E>(Ok(m_value));
    }
    
    template <typename E>
    constexpr operator Result<T, E>() && {
        return Restult<T, E>(Ok(std::move(m_value)));
    }

private:
    T m_value;
};

template <typename E>
struct Err {
    explicit constexpr Err(const E& value) : m_value(value) {}
    explicit constexpr Err(E&& value) : m_value(std::move(value)) {}

    constexpr const E& value() const& { return m_value; }
    constexpr E&& value() && { return std::move(m_value); }

private:
    E m_value;
};

template <typename T, typename E>
class Result {
public:
    Result() 
        : m_ok(false) {
    }
    
    Result(Ok<T> value) : m_data(value.value()), m_ok(true) {}
    Result(Err<E> value) : m_data(value.value()), m_ok(false) {}

    ~Result() {

    }
    Result(const Result& other) {
        m_data = other.m_data;
    }
    Result(Result&& other) {
        std::swap(other.m_ok, this->m_ok);
        std::swap(other.m_data, this->m_data);
    }

    Result& operator=(Result other) {
        m_data = other.m_data;
        m_ok = other.m_ok;
        return *this;
    }

    /// @brief If Result is Ok(), return the value. 
    /// Otherwise, panic
    constexpr T&& unwrap() {
        if (m_ok) {
            return std::get<T>(std::move(m_data));
        }

        terminate("Panic: unwrap failed on Result");
        std::exit(1);
    }


    bool is_ok() const { return this->m_ok == true; }
    bool is_err() const { return this->m_ok == false; }

private:
    std::variant<T, E> m_data;
    bool m_ok;

    /// @brief Panic and termiante program if Result is invalid
    void terminate(const std::string& msg) {
        std::cerr << msg << std::endl;
        std::terminate();
    }
};

} // namespace result
} // namespace viper
