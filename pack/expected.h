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
struct Unexpected;

// ===========================================================================================================

/// Utilite class simple implemented expected object (Value or error)
/// See http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0323r9.html
template <typename T, typename ErrorT = string_t>
class Expected
{
public:
    constexpr Expected() = delete;
    constexpr Expected(const T& value) noexcept;
    constexpr Expected(T&& value) noexcept;
    constexpr Expected(Expected&& other) noexcept;
    template <typename UnErrorT>
    constexpr Expected(Unexpected<UnErrorT>&& unex) noexcept;
    template <typename UnErrorT>
    constexpr Expected(const Unexpected<UnErrorT>& unex) noexcept;
    ~Expected();

    Expected(const Expected&)            = delete;
    Expected& operator=(const Expected&) = delete;

    constexpr const T&      value() const& noexcept;
    constexpr T&            value() & noexcept;
    constexpr const T&&     value() const&& noexcept;
    constexpr T&&           value() && noexcept;
    constexpr const ErrorT& error() const& noexcept;

    constexpr bool     isValid() const noexcept;
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
class Expected<void, ErrorT>
{
public:
    Expected() noexcept;
    template <typename UnErrorT>
    Expected(Unexpected<UnErrorT>&& unex) noexcept;
    template <typename UnErrorT>
    Expected(const Unexpected<UnErrorT>& unex) noexcept;

    Expected(const Expected&)            = delete;
    Expected& operator=(const Expected&) = delete;

    constexpr bool isValid() const noexcept;
    constexpr      operator bool() const noexcept;

    constexpr const ErrorT& error() const& noexcept;

private:
    std::optional<ErrorT> m_error;
    bool                  m_isError = false;
};

// ===========================================================================================================

template <typename ErrorT = string_t>
struct Unexpected
{
    constexpr Unexpected(const ErrorT& value) noexcept;
    ErrorT message;
};

// ===========================================================================================================

template <typename ErrorT>
constexpr Unexpected<ErrorT>::Unexpected(const ErrorT& value) noexcept
    : message(value)
{
}

template <typename ErrorT, typename = std::enable_if_t<!std::is_convertible_v<ErrorT, string_t>>>
inline Unexpected<ErrorT> unexpected(const ErrorT& error = {})
{
    return Unexpected<ErrorT>(error);
}

template <typename>
inline Unexpected<string_t> unexpected(const string_t& error)
{
    return Unexpected<string_t>(error);
}

#ifdef WITH_QTSTRING
inline Unexpected<string_t> unexpected(const char* error)
{
    return Unexpected<string_t>(QLatin1String(error));
}
#endif

template <typename... Args>
inline Unexpected<string_t> unexpected(const string_t& fmt, const Args&... args)
{
    try {
#ifdef WITH_QTSTRING
        return QString::fromStdString(fmt::format(fmt.toStdString(), args...));
#else
        return {fmt::format(fmt, args...)};
#endif
    } catch (const fmt::format_error&) {
        assert("Format error");
        return fmt;
    }
}

// ===========================================================================================================

template <typename T, typename ErrorT>
constexpr Expected<T, ErrorT>::Expected(const T& value) noexcept
    : m_value(value)
{
}

template <typename T, typename ErrorT>
constexpr Expected<T, ErrorT>::Expected(T&& value) noexcept
    : m_value(std::move(value))
{
}

template <typename T, typename ErrorT>
constexpr Expected<T, ErrorT>::Expected(Expected&& other) noexcept
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
constexpr Expected<T, ErrorT>::Expected(Unexpected<UnErrorT>&& unex) noexcept
    : m_error(std::move(unex.message))
    , m_isError(true)
{
}

template <typename T, typename ErrorT>
template <typename UnErrorT>
constexpr Expected<T, ErrorT>::Expected(const Unexpected<UnErrorT>& unex) noexcept
    : m_error(unex.message)
    , m_isError(true)
{
}

template <typename T, typename ErrorT>
Expected<T, ErrorT>::~Expected()
{
    if (m_isError) {
        m_error.~ErrorT();
    } else {
        m_value.~T();
    }
}

template <typename T, typename ErrorT>
constexpr const T& Expected<T, ErrorT>::value() const& noexcept
{
    assert(!m_isError);
    return m_value;
}

template <typename T, typename ErrorT>
constexpr T& Expected<T, ErrorT>::value() & noexcept
{
    assert(!m_isError);
    return m_value;
}

template <typename T, typename ErrorT>
constexpr const T&& Expected<T, ErrorT>::value() const&& noexcept
{
    assert(!m_isError);
    return std::move(m_value);
}

template <typename T, typename ErrorT>
constexpr T&& Expected<T, ErrorT>::value() && noexcept
{
    assert(!m_isError);
    return std::move(m_value);
}

template <typename T, typename ErrorT>
constexpr const ErrorT& Expected<T, ErrorT>::error() const& noexcept
{
    assert(m_isError);
    return m_error;
}

template <typename T, typename ErrorT>
constexpr bool Expected<T, ErrorT>::isValid() const noexcept
{
    return !m_isError;
}

template <typename T, typename ErrorT>
constexpr Expected<T, ErrorT>::operator bool() const noexcept
{
    return !m_isError;
}

template <typename T, typename ErrorT>
constexpr const T& Expected<T, ErrorT>::operator*() const& noexcept
{
    assert(!m_isError);
    return m_value;
}

template <typename T, typename ErrorT>
constexpr T& Expected<T, ErrorT>::operator*() & noexcept
{
    assert(!m_isError);
    return m_value;
}

template <typename T, typename ErrorT>
constexpr const T&& Expected<T, ErrorT>::operator*() const&& noexcept
{
    assert(!m_isError);
    return std::move(m_value);
}

template <typename T, typename ErrorT>
constexpr T&& Expected<T, ErrorT>::operator*() && noexcept
{
    assert(!m_isError);
    return std::move(m_value);
}


template <typename T, typename ErrorT>
constexpr const T* Expected<T, ErrorT>::operator->() const noexcept
{
    assert(!m_isError);
    return &m_value;
}

template <typename T, typename ErrorT>
constexpr T* Expected<T, ErrorT>::operator->() noexcept
{
    assert(!m_isError);
    return &m_value;
}

// ===========================================================================================================

template <typename ErrorT>
inline Expected<void, ErrorT>::Expected() noexcept
{
}

template <typename ErrorT>
template <typename UnErrorT>
inline Expected<void, ErrorT>::Expected(Unexpected<UnErrorT>&& unex) noexcept
    : m_error(std::move(unex.message))
    , m_isError(true)
{
}

template <typename ErrorT>
template <typename UnErrorT>
inline Expected<void, ErrorT>::Expected(const Unexpected<UnErrorT>& unex) noexcept
    : m_error(unex.message)
    , m_isError(true)
{
}

template <typename ErrorT>
inline constexpr bool Expected<void, ErrorT>::isValid() const noexcept
{
    return !m_isError;
}

template <typename ErrorT>
inline constexpr Expected<void, ErrorT>::operator bool() const noexcept
{
    return !m_isError;
}

template <typename ErrorT>
inline constexpr const ErrorT& Expected<void, ErrorT>::error() const& noexcept
{
    assert(m_error != std::nullopt);
    return *m_error;
}

// ===========================================================================================================

} // namespace pack
