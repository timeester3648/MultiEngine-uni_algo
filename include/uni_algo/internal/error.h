/* C++ Standard Library wrapper for Unicode Algorithms Implementation.
 * License: Public Domain or MIT - choose whatever you want.
 * See LICENSE.md */

#ifndef UNI_ALGO_INTERNAL_ERROR_H_UAIH
#define UNI_ALGO_INTERNAL_ERROR_H_UAIH

#include <cassert>
#ifdef UNI_ALGO_EXPERIMENTAL
#include <string_view>
#endif

#include "../config.h"

namespace una {

class error
{
public:
    enum class code : unsigned short {success = 0, ill_formed_utf};
public:
    constexpr error() noexcept = default;
    constexpr error(error::code ec) noexcept : error_code{ec} {}
    constexpr error(error::code ec, std::size_t p) noexcept : position{p}, error_code{ec} {}
    constexpr explicit operator bool() const noexcept { return error_code != error::code::success; }
    constexpr void reset() noexcept { error_code = error::code::success; position = detail::impl_npos; }
    constexpr bool has_pos() const noexcept { return position != detail::impl_npos; }
    constexpr std::size_t pos() const noexcept { assert(operator bool()); assert(has_pos()); return position; }
    constexpr error::code get_code() const noexcept { return error_code; }
    friend constexpr bool operator==(const error& x, const error::code& y) noexcept { return x.error_code == y; }
    friend constexpr bool operator!=(const error& x, const error::code& y) noexcept { return x.error_code != y; }
    friend constexpr bool operator==(const error::code& x, const error& y) noexcept { return x == y.error_code; }
    friend constexpr bool operator!=(const error::code& x, const error& y) noexcept { return x != y.error_code; }
#ifdef UNI_ALGO_EXPERIMENTAL
    std::string_view to_string_view() const noexcept
    {
        switch(error_code)
        {
            case code::success: return "success";
            case code::ill_formed_utf: return "ill-formed UTF";
            default: return "invalid error code";
        }
    }
#endif // UNI_ALGO_EXPERIMENTAL
private:
    std::size_t position = detail::impl_npos;
    error::code error_code = error::code::success;
};

} // namespace una

#endif // UNI_ALGO_INTERNAL_ERROR_H_UAIH
