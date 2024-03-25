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

// For the Ok type of a result
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

// For the Err type of a result
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

    // For types that are coercible like pointers to inherited classes
    template<typename U>
    Result(Ok<U> value) : m_data(static_cast<U>(value.value())), m_ok(true) {}

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

    // [[ Conversion operators ]] //

    /// @brief If Result is Ok(), return the value. 
    /// Otherwise, panic
    constexpr T&& unwrap() {
        if (m_ok) {
            return std::get<T>(std::move(m_data));
        }

        terminate("Panic: unwrap failed on Result");
        std::exit(1);
    }

    constexpr E&& unwrap_err() {
        if(!m_ok) {
            this->terminate("Called `unwrap_err` on Err value");
        }
        return std::get<E>(std::move(m_data));
    }

    constexpr T&& unwrap_or(T&& val) {
        if (m_ok) {
            return std::get<T>(std::move(m_data));
        }
        return val;
    }

    constexpr T&& unwrap_or_default() {
        static_assert(
            std::is_default_constructible<T>::value,
            "`unwrap_or_default` requires <T> to be default construbtible"
        );

        if(m_ok) {
            return std::get<T>(std::move(m_data));
        }
        return T();
    }

    constexpr E&& unwrap_err_or(E && error) {
        static_assert(
            std::is_default_constructible<E>::value,
            "`unwrap_or_default` requires <E> to be default construbtible"
        );

        if (!m_ok) {
            return E();
        }
        return std::get<E>(std::move(m_data));
    }

    constexpr E&& expect_err(const std::string_view& msg) {
        if(m_ok) {
            this->terminate(msg);
        }

        return std::get<E>(std::move(m_data));
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
