#pragma once

#include "defines.h"
#include <functional>
#include <optional>
#include <ostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>

namespace viper {

template <typename T>
using optional = std::optional<T>;
using nullopt_t = std::nullopt_t;
inline constexpr nullopt_t nullopt = std::nullopt;

template <typename T>
inline std::ostream& operator<<(
    std::ostream& stream,
    const std::reference_wrapper<T>& obj
);

enum class ResultKind : u8 {
    Ok = 0,
    Err = 1
};

template <typename T, typename E>
class [[nodiscard]] Result;

struct ok_tag_t {};
struct err_tag_t {};
struct unit_t {};

inline constexpr ok_tag_t ok_tag = ok_tag_t{};
inline constexpr err_tag_t err_tag = err_tag_t{};
inline constexpr unit_t unit = unit_t{};

template <typename T>
struct is_result : std::false_type {};

template <typename T, typename E>
struct is_result<Result<T, E>> : std::true_type {};

inline constexpr bool operator==(unit_t, unit_t) { return true; }
inline constexpr bool operator!=(unit_t, unit_t) { return false; }

template <typename T>
class Err {
    public:
        using value_type = T;

        explicit constexpr Err(const T& value) : m_value(value) {}
        explicit constexpr Err(T&& value) : m_value(std::move(value)) {}

        constexpr const T& value() const& { return m_value; }
        constexpr T&& value() && { return std::move(m_value); }

    private:
        T m_value;

};  

template <typename T>
class Ok {
    public:
        using value_type = T;

        explicit constexpr Ok(const T& value) : m_value(value) {}
        explicit constexpr Ok(T&& value) : m_value(std::move(value)) {}

        constexpr const T& value() const& { return m_value; }
        constexpr T&& value() && { return std::move(m_value); }

        template <typename E>
        constexpr operator Result<T, E>() const& {
            return Result<T, E>(Ok(m_value));
        }

        template <typename E>
        constexpr operator Result<T, E>() && {
            return Result<T, E>(Ok(std::move(m_value)));
        }



    private:
        T m_value;

};

template <>
class Ok<unit_t> {
    public:
    using value_type = unit_t;

        constexpr Ok() = default;
        constexpr Ok(unit_t) {}

        constexpr const unit_t value() const { return {}; }
};

Ok()->Ok<unit_t>;

// Result Storage 
template <typename T, typename E>
class ResultStorage {
    using DecayT = std::decay_t<T>;
    using DecayE = std::decay_t<E>;

public:
    using value_type = T;
    using error_type = E;
    using data_type = std::aligned_union<1, T, E>; // subject to change to std::variant

    ResultStorage() = delete;

    // Store valid (Ok) data
    template <typename... Args>
    constexpr ResultStorage(ok_tag_t, Args&&... args) {
        if constexpr(!std::is_same<T, unit_t>::value) {
            new(&m_data) DecayT(std::forward<Args>(args)...);
        }
        m_tag = ResultKind::Ok;
    }


    // Store error data
    template <typename... Args>
    constexpr ResultStorage(err_tag_t, Args&&... args) {
        new(&m_data) DecayE(std::forward<Args>(args)...);
        m_tag = ResultKind::Err;
    }

    constexpr ResultStorage(Ok<T> val) {
        if constexpr(!std::is_same<T, unit_t>::value) {
            new(&m_data) DecayT(std::move(val).value());
        }
        m_tag = ResultKind::Ok;
    }

    constexpr ResultStorage(Err<E> val) {
        new(&m_data) DecayE(std::move(val).value());
        m_tag = ResultKind::Err;
    }

    constexpr ResultStorage(const ResultStorage<T, E>& rhs) noexcept (
            std::is_nothrow_copy_constructible<T>::value&&
            std::is_nothrow_copy_constructible<E>::value)
    : m_tag(rhs.m_tag) {
        if(kind() == ResultKind::Ok) {
            if constexpr(!std::is_same<T, unit_t>::value) {
                new(&m_data) DecayT(rhs.get<T>());
            }
        } else {
            new(&m_data) DecayE(rhs.get<T>());
        }
    }

    constexpr ResultStorage(const ResultStorage<T, E>&& rhs) noexcept (
            std::is_nothrow_copy_constructible<T>::value&&
            std::is_nothrow_copy_constructible<E>::value)
    : m_tag(rhs.m_tag) {
        if(kind() == ResultKind::Ok) {
            if constexpr(!std::is_same<T, unit_t>::value) {
                new(&m_data) DecayT(std::move(rhs).template get<T>());
            }
        } else {
            new(&m_data) DecayE(std::move(rhs).template get<T>());
        }
    }

    constexpr ResultStorage& operator=(const ResultStorage<T, E>& rhs) noexcept (
            std::is_nothrow_copy_constructible<T>::value&&
            std::is_nothrow_copy_constructible<E>::value)
    {
        destroy();
        m_tag = rhs.m_tag;

        if (kind() == ResultKind::Ok) {
            T& val = get<T>();
            val = rhs.get<T>();
        } else {
            E& val = get<T>();
            val = rhs.get<E>();
        }
    }

    constexpr ResultStorage& operator=(const ResultStorage<T, E>&& rhs) noexcept (
            std::is_nothrow_copy_constructible<T>::value&&
            std::is_nothrow_copy_constructible<E>::value)
    {
        destroy();
        m_tag = rhs.m_tag;

        if (kind() == ResultKind::Ok) {
            T& val = get<T>();
            val = std::move(rhs).template get<T>();
        } else {
            E& val = get<T>();
            val = std::move(rhs).template get<E>();
        }
    }


    template <typename U>
    constexpr const U& get() const& noexcept {
        static_assert(std::is_same<T, U>::value || std::is_same<E, U>::value);
        return *reinterpret_cast<U*>(&m_data);
    }
    
    template <typename U>
    constexpr const U& get() & noexcept {
        static_assert(std::is_same<T, U>::value || std::is_same<E, U>::value);
        return *reinterpret_cast<U*>(&m_data);
    }

    template <typename U>
    constexpr U&& get() && noexcept {
        static_assert(std::is_same<T, U>::value || std::is_same<E, U>::value);
        return std::move(*reinterpret_cast<U*>(&m_data));
    }

    constexpr ResultKind kind() const noexcept {
        return m_tag;
    }

    ~ResultStorage() {
        destroy();
    }

private:
    void destroy() {
        switch (m_tag) {
            case ResultKind::Ok: 
                get<T>().~T();
                break;
            case ResultKind::Err: 
                get<E>().~E();
                break;
        }
    }

    ResultKind m_tag;
    data_type m_data;
};

template <typename T, typename E>
class [[nodiscard]] VResult {
    public:
        using value_type = T;
        using error_type = E;

        static_assert(
            std::is_same<std::remove_reference<T>, T>::value,
            "Result<T, E> cannot store reference types. "
            "Try using `std::reference_wrapper`"
        );
        static_assert(
            std::is_same<std::remove_reference<E>, E>::value,
            "Result<T, E> cannot store reference types. "
            "Try using `std::reference_wrapper`"
        );

        static_assert(
            !std::is_same<T, void>::value,
            "Cannot create VResult<T, E> object with T=void. "
            "Use unit_t instead"
        );
        static_assert(
            !std::is_same<E, void>::value,
            "Cannot create VResult<T, E> object with E=void. "
            "Use std::optional<T> if you want this behavior"
        );

        constexpr VResult() {
            static_assert(
                std::is_default_constructible<T>::value,
                "VResult<T, E> may only be default constructed if T is default constructible. "
            );
            
        }

    private:

};

} // namespace viper
