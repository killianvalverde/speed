/* speed - Generic C++ library.
 * Copyright (C) 2015-2026 Killian Valverde.
 *
 * This file is part of speed.
 *
 * speed is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * speed is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with speed. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file user_id.hpp
 * @brief Defines the user_id class.
 * @author Killian Valverde
 * @date 2026-06-10
 */

#pragma once

#include "../platform/platform.hpp"

#include <compare>
#include <concepts>
#include <limits>
#include <string>
#include <type_traits>
#include <utility>

#include "../codecs/operations.hpp"

namespace speed::system::process {

/**
 * @brief Represents a platform-independent user identifier.
 *
 * This class encapsulates the native user identifier used by the underlying operating system
 * while providing a uniform interface across supported platforms.
 *
 * On Windows, the native identifier is stored as a Security Identifier (SID) string representation.
 *
 * On POSIX systems, the native identifier is stored as a native uid_t value.
 *
 * Instances of this class support comparison, conversion to a string representation, and access
 * to the underlying native identifier.
 */
class user_id
{
public:
    /** Native user identifier type. */
    using native_type =
#ifdef SPEED_WINAPI
    std::wstring;
#else
    ::uid_t;
#endif

    /**
     * @brief Constructs an empty user identifier.
     *
     * The resulting identifier does not contain a valid native user identifier and can be
     * detected using is_empty() or operator bool().
     */
    constexpr user_id() = default;

    /**
     * @brief Constructs a user identifier from a native identifier value.
     *
     * @tparam NativeT Type of the native identifier.
     *
     * @param native Native user identifier value.
     */
    template<typename NativeT>
        requires (!std::same_as<std::remove_cvref_t<NativeT>, user_id> &&
            std::constructible_from<native_type, NativeT&&>)
    explicit user_id(NativeT&& native)
        : native_(std::forward<NativeT>(native))
    {
    }

    /**
     * @brief Compares two user identifiers.
     *
     * Comparison is performed using the underlying native identifier.
     *
     * @param other User identifier to compare against.
     * @return The result of the comparison.
     */
    auto operator<=>(const user_id& other) const = default;

    /**
     * @brief Checks whether the identifier is non-empty.
     *
     * @return true if the identifier contains a valid native value; otherwise false.
     */
    [[nodiscard]] explicit operator bool() const noexcept
    {
        return !is_empty();
    }

    /**
     * @brief Assigns a native user identifier value.
     *
     * @tparam NativeT Type of the native identifier.
     *
     * @param native Native user identifier value to assign.
     * @return true if the assignment succeeded; otherwise false.
     */
    template<typename NativeT>
        requires std::constructible_from<native_type, NativeT&&>
    bool assign(NativeT&& native) noexcept
    {
        try
        {
            native_ = std::forward<NativeT>(native);
            return true;
        }
        catch (const std::bad_alloc&)
        {
            return false;
        }
    }

    /**
     * @brief Checks whether the identifier is empty.
     *
     * A default-constructed user identifier is considered empty.
     *
     * @return true if the identifier is empty; otherwise false.
     */
    [[nodiscard]] bool is_empty() const noexcept
    {
#ifdef SPEED_WINAPI
        return native_.empty();
#else
        return native_ == std::numeric_limits<::pid_t>::max();
#endif
    }

    /**
     * @brief Returns the native user identifier.
     *
     * @return Constant reference to the underlying native identifier.
     */
    [[nodiscard]] const native_type& get_native() const noexcept
    {
        return native_;
    }

    /**
     * @brief Converts the identifier to a string representation.
     *
     * On Windows, this returns the SID string.
     * On POSIX systems, this returns the decimal representation of the native user identifier.
     *
     * @return String representation of the identifier.
     */
    [[nodiscard]] std::string to_string() const
    {
#ifdef SPEED_WINAPI
        std::string result;
        codecs::convert_wstring_to_utf8(native_.c_str(), result);
        return result;
#else
        return std::to_string(native_);
#endif
    }

private:
    /** Native user identifier. */
    native_type native_ =
#ifdef SPEED_WINAPI
        {};
#else
    std::numeric_limits<::pid_t>::max();
#endif
};

}
