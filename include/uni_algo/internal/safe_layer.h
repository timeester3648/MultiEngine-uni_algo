/* C++ Standard Library wrapper for Unicode Algorithms Implementation.
 * License: Public Domain or MIT - choose whatever you want.
 * See LICENSE.md */

#ifndef UNI_ALGO_INTERNAL_SAFE_LAYER_H_UAIH
#define UNI_ALGO_INTERNAL_SAFE_LAYER_H_UAIH

#include <cstdlib>
#include <array>

namespace uni::detail {

namespace safe {

inline void kms()
{
    std::abort();
}

template<typename T, std::size_t N>
struct array
{
    static_assert(N != 0, "Low-level must never try to use 0 sized arrays");

    static_assert(!std::is_integral_v<T> || // For multidimensional arrays
                  std::is_same_v<T, char>           ||
                  std::is_same_v<T, unsigned char>  ||
                  std::is_same_v<T, unsigned short> ||
                  std::is_same_v<T, char32_t>,
                 "Low-level must never use disallowed types for arrays");

    using value_type      = T;
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference       = value_type&;
    using const_reference = const value_type&;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;
    using iterator        = value_type*;
    using const_iterator  = const value_type*;

    T internal_array[N];

    constexpr reference operator[](size_type pos) noexcept
    { if (pos >= N) kms(); return internal_array[pos]; }
    constexpr const_reference operator[](size_type pos) const noexcept
    { if (pos >= N) kms(); return internal_array[pos]; }
    // Low-level must never use the folowing functions but they might be helpfull for tests
    constexpr size_type size() const noexcept { return N; }
    constexpr size_type max_size() const noexcept { return N; }
    constexpr bool empty() const noexcept { return N == 0; }
    constexpr pointer data() noexcept { return internal_array; }
    constexpr const_pointer data() const noexcept { return internal_array; }
    constexpr iterator begin() noexcept { return internal_array; }
    constexpr const_iterator begin() const noexcept { return internal_array; }
    constexpr iterator end() noexcept { return internal_array + N; }
    constexpr const_iterator end() const noexcept { return internal_array + N; }
    // Low-level must never try to copy arrays
    // But this doesn't work without copy constructor and copy constructor breaks aggregate
    // https://timsong-cpp.github.io/cppwp/n4659/dcl.init.aggr#1.1
    //constexpr type_array& operator=(const type_array&) = delete;
    // Low-level must never try to compare arrays so no comparison operators
};
static_assert(std::is_aggregate_v<array<char, 1>>, "safe::array must be aggregate");

} // namespace safe

#if 0
template<typename T, std::size_t N>
using type_array = safe::array<T, N>;
#else
template<typename T, std::size_t N>
using type_array = std::array<T, N>;
#endif

} // namespace uni::detail

#endif // UNI_ALGO_INTERNAL_SAFE_LAYER_H_UAIH