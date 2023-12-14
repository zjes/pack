/* =========================================================================================================================================
    ____ __ _ ____ __  __
   |    |  ` |    |  /  /
   | |  | |  | |__|    /
   | ___| |  | |  |    \
   |_|  |__,_|____|__\__\ DSO library

   Copyright (C) 2020 Eaton
   Copyright (C) 2020-2022 zJes

   This program is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
========================================================================================================================================= */
#pragma once

#include "pack/types.h"
#include "pack/utils.h"
#include <cassert>
#include <fmt/format.h>
#include <optional>
#ifdef WITH_QTSTRING
#include <QString>
#else
#include <string>
#endif

namespace pack {

template <typename>
struct unexpected;

// ===========================================================================================================

/// Utilite class simple implemented expected object (Value or error)
/// See http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0323r9.html
template <typename T, typename ErrorT = UString>
class expected
{
    static_assert(!std::is_reference_v<T>);
    static_assert(!std::is_function_v<T>);

public:
    constexpr expected() = delete;
    constexpr expected(const T& value) noexcept;
    constexpr expected(T&& value) noexcept;
    constexpr expected(expected&& other) noexcept;
    template <typename UnErrorT>
    constexpr expected(unexpected<UnErrorT>&& unex) noexcept;
    template <typename UnErrorT>
    constexpr expected(const unexpected<UnErrorT>& unex) noexcept;
    ~expected();

    expected(const expected&)            = delete;
    expected& operator=(const expected&) = delete;

    constexpr const T&      value() const& noexcept;
    constexpr T&            value() & noexcept;
    constexpr const T&&     value() const&& noexcept;
    constexpr T&&           value() && noexcept;
    constexpr const ErrorT& error() const& noexcept;

    constexpr bool isValid() const noexcept;
    explicit constexpr operator bool() const noexcept;

    constexpr const T&  operator*() const& noexcept;
    constexpr T&        operator*() & noexcept;
    constexpr const T&& operator*() const&& noexcept;
    constexpr T&&       operator*() && noexcept;
    constexpr const T*  operator->() const noexcept;
    constexpr T*        operator->() noexcept;

private:
    union {
        T      m_value;
        ErrorT m_error;
    };
    bool m_isError = false;
};

template <typename ErrorT>
class expected<void, ErrorT>
{
public:
    expected() noexcept;
    template <typename UnErrorT>
    expected(unexpected<UnErrorT>&& unex) noexcept;
    template <typename UnErrorT>
    expected(const unexpected<UnErrorT>& unex) noexcept;

    expected(const expected&)            = delete;
    expected& operator=(const expected&) = delete;

    constexpr bool isValid() const noexcept;
    constexpr operator bool() const noexcept;

    constexpr const ErrorT& error() const& noexcept;

private:
    std::optional<ErrorT> m_error;
    bool                  m_isError = false;
};

// ===========================================================================================================

template <typename ErrorT = UString>
struct unexpected
{
    constexpr unexpected(const ErrorT& value) noexcept;
    template <typename... Args>
    consteval unexpected(const fmt::format_string<Args...>& fmt, const Args&... args) noexcept;
    template <typename... Args>
    consteval unexpected(const UString& fmt, const Args&... args) noexcept;

    ErrorT message;
};

// ===========================================================================================================

template <typename ErrorT>
constexpr unexpected<ErrorT>::unexpected(const ErrorT& value) noexcept
    : message(value)
{
}

template <>
template <typename... Args>
consteval unexpected<UString>::unexpected(const fmt::format_string<Args...>& fmt, const Args&... args) noexcept
    : message(format(fmt, args...))
{
}

template <>
template <typename... Args>
consteval unexpected<UString>::unexpected(const UString& fmt, const Args&... args) noexcept
    : message(format(fmt, args...))
{
}

// ===========================================================================================================

template <typename T, typename ErrorT>
constexpr expected<T, ErrorT>::expected(const T& value) noexcept
    : m_value(value)
{
}

template <typename T, typename ErrorT>
constexpr expected<T, ErrorT>::expected(T&& value) noexcept
    : m_value(std::move(value))
{
}

template <typename T, typename ErrorT>
constexpr expected<T, ErrorT>::expected(expected&& other) noexcept
{
    if (other.m_isError) {
        m_isError = true;
        m_error   = std::move(other.m_error);
    } else {
        m_value = std::move(other.m_value);
    }
}

template <typename T, typename ErrorT>
template <typename UnErrorT>
constexpr expected<T, ErrorT>::expected(unexpected<UnErrorT>&& unex) noexcept
    : m_error(std::move(unex.message))
    , m_isError(true)
{
}

template <typename T, typename ErrorT>
template <typename UnErrorT>
constexpr expected<T, ErrorT>::expected(const unexpected<UnErrorT>& unex) noexcept
    : m_error(unex.message)
    , m_isError(true)
{
}

template <typename T, typename ErrorT>
expected<T, ErrorT>::~expected()
{
    if (m_isError) {
        m_error.~ErrorT();
    } else {
        m_value.~T();
    }
}

template <typename T, typename ErrorT>
constexpr const T& expected<T, ErrorT>::value() const& noexcept
{
    assert(!m_isError);
    return m_value;
}

template <typename T, typename ErrorT>
constexpr T& expected<T, ErrorT>::value() & noexcept
{
    assert(!m_isError);
    return m_value;
}

template <typename T, typename ErrorT>
constexpr const T&& expected<T, ErrorT>::value() const&& noexcept
{
    assert(!m_isError);
    return std::move(m_value);
}

template <typename T, typename ErrorT>
constexpr T&& expected<T, ErrorT>::value() && noexcept
{
    assert(!m_isError);
    return std::move(m_value);
}

template <typename T, typename ErrorT>
constexpr const ErrorT& expected<T, ErrorT>::error() const& noexcept
{
    assert(m_isError);
    return m_error;
}

template <typename T, typename ErrorT>
constexpr bool expected<T, ErrorT>::isValid() const noexcept
{
    return !m_isError;
}

template <typename T, typename ErrorT>
constexpr expected<T, ErrorT>::operator bool() const noexcept
{
    return !m_isError;
}

template <typename T, typename ErrorT>
constexpr const T& expected<T, ErrorT>::operator*() const& noexcept
{
    assert(!m_isError);
    return m_value;
}

template <typename T, typename ErrorT>
constexpr T& expected<T, ErrorT>::operator*() & noexcept
{
    assert(!m_isError);
    return m_value;
}

template <typename T, typename ErrorT>
constexpr const T&& expected<T, ErrorT>::operator*() const&& noexcept
{
    assert(!m_isError);
    return std::move(m_value);
}

template <typename T, typename ErrorT>
constexpr T&& expected<T, ErrorT>::operator*() && noexcept
{
    assert(!m_isError);
    return std::move(m_value);
}


template <typename T, typename ErrorT>
constexpr const T* expected<T, ErrorT>::operator->() const noexcept
{
    assert(!m_isError);
    return &m_value;
}

template <typename T, typename ErrorT>
constexpr T* expected<T, ErrorT>::operator->() noexcept
{
    assert(!m_isError);
    return &m_value;
}

// ===========================================================================================================

template <typename ErrorT>
inline expected<void, ErrorT>::expected() noexcept
{
}

template <typename ErrorT>
template <typename UnErrorT>
inline expected<void, ErrorT>::expected(unexpected<UnErrorT>&& unex) noexcept
    : m_error(std::move(unex.message))
    , m_isError(true)
{
}

template <typename ErrorT>
template <typename UnErrorT>
inline expected<void, ErrorT>::expected(const unexpected<UnErrorT>& unex) noexcept
    : m_error(unex.message)
    , m_isError(true)
{
}

template <typename ErrorT>
inline constexpr bool expected<void, ErrorT>::isValid() const noexcept
{
    return !m_isError;
}

template <typename ErrorT>
inline constexpr expected<void, ErrorT>::operator bool() const noexcept
{
    return !m_isError;
}

template <typename ErrorT>
inline constexpr const ErrorT& expected<void, ErrorT>::error() const& noexcept
{
    assert(m_error != std::nullopt);
    return *m_error;
}

// ===========================================================================================================

} // namespace pack
