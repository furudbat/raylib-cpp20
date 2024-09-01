/// https://github.com/alexgunkel/strongtypes/blob/master/LICENSE
#ifndef LIST_STRONG_TYPES_H
#define LIST_STRONG_TYPES_H

#include <type_traits>
#include <utility>

namespace strong_types {
enum class ConversionType : uint8_t {
    None,
    Explicit,
    Implicit
};

template<typename T, typename P, ConversionType CONVERSION = ConversionType::None>
class Type {
    T obj;
public:
    constexpr Type() = default;

    constexpr Type(const Type& other) = default;

    template<ConversionType USE = CONVERSION>
    constexpr explicit Type(const T&& o) noexcept
        requires(USE != ConversionType::Implicit)
        : obj{o} {}

    template<ConversionType USE = CONVERSION>
    constexpr Type(const T&& o) noexcept
        requires(USE == ConversionType::Implicit)
        : obj{o} {}

    template<ConversionType USE = CONVERSION>
    constexpr explicit Type(const T& o) noexcept
        requires(USE != ConversionType::Implicit)
        : obj{o} {}

    template<ConversionType USE = CONVERSION>
    constexpr Type(const T& o) noexcept
        requires(USE == ConversionType::Implicit)
        : obj{o} {}

    constexpr Type& operator=(const Type<T, P>& other) noexcept {
        if (&other == this) {
            return *this;
        }

        obj = other.obj;
        return *this;
    }

    template<ConversionType USE = CONVERSION>
    constexpr explicit operator T() const noexcept
        requires(USE == ConversionType::Explicit)
    {
        return obj;
    }

    template<ConversionType USE = CONVERSION>
    constexpr operator T() const noexcept
        requires(USE == ConversionType::Implicit)
    {
        return obj;
    }

    inline const T& get() const noexcept { return obj; }

    bool operator==(const Type& rhs) const noexcept { return obj == rhs.obj; }

    bool operator!=(const Type& rhs) const noexcept { return !(rhs == *this); }

    bool operator<(const Type& rhs) const noexcept { return obj < rhs.obj; }

    bool operator>(const Type& rhs) const { return rhs < *this; }

    bool operator<=(const Type& rhs) const { return !(rhs < *this); }

    bool operator>=(const Type& rhs) const { return !(*this < rhs); }

    /*
    friend std::ostream &operator<<(std::ostream &os, const Type &type) {
        os << type.obj;
        return os;
    }

    friend std::istream &operator>>(std::istream &is, Type &type) {
        is >> type;
        return is;
    }
    */

    constexpr Type<T, P> operator+(const Type<T, P>& other) const { return Type<T, P>{obj + other.obj}; }

    template<class Integer>
    friend constexpr Type<T, P> operator*(const Type<T, P>& left, const Integer& right)
        requires(std::is_integral_v<Integer>)
    {
        auto result = left;
        for (Integer i = 1; i < right; ++i) {
            result.obj += left.obj;
        }

        return result;
    }

    template<class Integer>
    friend constexpr Type<T, P> operator*(const Integer& left, const Type<T, P>& right)
        requires(std::is_integral_v<Integer>)
    {
        auto result = right;
        for (Integer i = 1; i < left; ++i) {
            result.obj += right.obj;
        }

        return result;
    }
};

template<class S, class T, class... Args>
static constexpr S emplace(Args&&... args) {
    return S(T(std::forward<Args>(args)...));
}
} // namespace strong_types

#define STRONG_TYPE(name, type) using name = ::strong_types::Type<type, struct Type##name>;
#define CONVERTIBLE_STRONG_TYPE(name, type) \
    using name = ::strong_types::Type<type, struct Type##name, strong_types::ConversionType::Explicit>;
#define IMPLICIT_STRONG_TYPE(name, type) \
    using name = ::strong_types::Type<type, struct Type##name, strong_types::ConversionType::Implicit>;

#endif // LIST_STRONG_TYPES_H