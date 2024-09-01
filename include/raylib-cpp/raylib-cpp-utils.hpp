/**
 * Utility for raylib-cpp.
 */
#ifndef RAYLIB_CPP_INCLUDE_RAYLIB_CPP_UTILS_HPP_
#define RAYLIB_CPP_INCLUDE_RAYLIB_CPP_UTILS_HPP_

#include <cstdint>
#include <cstdlib>
#include <memory>
#include <span>
#if __has_include(<gsl/gsl>)
#include <gsl/gsl>
#endif

/**
 * Allow changing the declare type for all raylib-cpp global functions. Defaults to static.
 */
#ifndef RLCPPAPI
#define RLCPPAPI static
// no inline needed for static (header) function
#define RLCPPAPI_INLINE static
#else
#define RLCPPAPI_INLINE RLCPPAPI inline
#endif

#ifndef GETTERSETTER
/**
 * A utility to build get and set methods on top of a property.
 *
 * @param type The type of the property.
 * @param method The human-readable name for the method.
 * @param name The machine-readable name of the property.
 */
#define GETTERSETTER(type, method, name)                                                       \
    /** Retrieves the name value for the object. @return The name value of the object. */      \
    constexpr type Get##method()& {                                                            \
        return name;                                                                           \
    }                                                                                          \
    constexpr type Get##method() const& {                                                      \
        return name;                                                                           \
    }                                                                                          \
    constexpr type Get##method()&& {                                                           \
        return std::move(name);                                                                \
    }                                                                                          \
    /** Sets the name value for the object. @param value The value of which to set name to. */ \
    constexpr void Set##method(type value) {                                                   \
        name = value;                                                                          \
    }
#endif
#ifndef GETTER
#define GETTER(type, method, name)                                                        \
    /** Retrieves the name value for the object. @return The name value of the object. */ \
    constexpr type Get##method()& {                                                       \
        return name;                                                                      \
    }                                                                                     \
    constexpr type Get##method() const& {                                                 \
        return name;                                                                      \
    }                                                                                     \
    constexpr type Get##method()&& {                                                      \
        return std::move(name);                                                           \
    }
#endif
#ifndef CONST_GETTER
#define CONST_GETTER(type, method, name)                                                  \
    /** Retrieves the name value for the object. @return The name value of the object. */ \
    constexpr const type Get##method() const {                                            \
        return name;                                                                      \
    }
#endif
#ifndef CONST_GETTERSETTER
#define CONST_GETTERSETTER(type, method, name)                                                 \
    /** Retrieves the name value for the object. @return The name value of the object. */      \
    constexpr const type Get##method() const& {                                                \
        return name;                                                                           \
    }                                                                                          \
    constexpr type Get##method()&& {                                                           \
        return std::move(name);                                                                \
    }                                                                                          \
    /** Sets the name value for the object. @param value The value of which to set name to. */ \
    constexpr void Set##method(type value) {                                                   \
        name = value;                                                                          \
    }
#endif
#ifndef SPAN_GETTER
#define SPAN_GETTER(type, method, name, size)                                             \
    /** Retrieves the name value for the object. @return The name value of the object. */ \
    constexpr std::span<const type> Get##method() const {                                 \
        return {name, static_cast<size_t>(size)};                                         \
    }                                                                                     \
    constexpr std::span<type> Get##method() {                                             \
        return {name, static_cast<size_t>(size)};                                         \
    }
#endif

#ifndef COMPOSITION_METHODE_CALL
#define COMPOSITION_METHODE_CALL(method, name)                     \
    constexpr auto method(auto&&... args) {                        \
        return name.method(std::forward<decltype(args)>(args)...); \
    }
#endif
#ifndef CONST_COMPOSITION_METHODE_CALL
#define CONST_COMPOSITION_METHODE_CALL(method, name)               \
    constexpr auto method(auto&&... args) const {                  \
        return name.method(std::forward<decltype(args)>(args)...); \
    }
#endif
#ifndef COMPOSITION_METHODE_CALL_RETURN_THIS
#define COMPOSITION_METHODE_CALL_RETURN_THIS(method, name)  \
    constexpr auto& method(auto&&... args) {                \
        name.method(std::forward<decltype(args)>(args)...); \
        return *this;                                       \
    }
#endif

#if defined(RAYLIB_CPP_INCLUDE_EXPECTED) || defined(RAYLIB_CPP_EXPECTED)
#ifdef RAYLIB_CPP_INCLUDE_EXPECTED
#include RAYLIB_CPP_INCLUDE_EXPECTED
#endif
#ifndef RAYLIB_CPP_EXPECTED
#error "define RAYLIB_CPP_EXPECTED required"
#endif
#ifndef RAYLIB_CPP_UNEXPECTED
#error "define RAYLIB_CPP_UNEXPECTED, when using RAYLIB_CPP_EXPECTED"
#endif
template<class T, class E>
using expected = RAYLIB_CPP_EXPECTED<T, E>;
template<class E>
using unexpected = RAYLIB_CPP_UNEXPECTED<E>;
#define RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(Error) return RAYLIB_CPP_UNEXPECTED(Error);
#define RAYLIB_CPP_RETURN_EXPECTED_VALUE(Value) return Value;
#define RAYLIB_CPP_RETURN_EXPECTED_VOID_VALUE(Value) return Value;
#define RAYLIB_CPP_RETURN_EXPECTED() return {};
#define RAYLIB_CPP_THROWS
#else
template<class T, class E>
using expected = void;
template<class E>
using unexpected = E;
#ifdef __cpp_exceptions
#define RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(Error) throw RaylibException(Error);
#define RAYLIB_CPP_RETURN_EXPECTED_VALUE(Value) return Value;
#define RAYLIB_CPP_RETURN_EXPECTED_VOID_VALUE(Value) return Value;
#define RAYLIB_CPP_RETURN_EXPECTED()
#define RAYLIB_CPP_THROWS noexcept(false)
#else
#define RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(Error) std::abort();
#define RAYLIB_CPP_RETURN_EXPECTED_VALUE(Value) return Value;
#define RAYLIB_CPP_RETURN_EXPECTED_VOID_VALUE(Value) Value;
#define RAYLIB_CPP_RETURN_EXPECTED()
#define RAYLIB_CPP_THROWS
#endif
#endif

namespace raylib {

#if __has_include(<gsl/gsl>)
using czstring = gsl::czstring;
#else
using czstring = const char*;
#endif

struct RayPointerDeleter {
    template<typename T>
    constexpr void operator()(T* arg) const {
        RL_FREE(arg);
    }
};

template<typename T>
using RayUniquePtr = std::unique_ptr<T, RayPointerDeleter>;

template<typename T>
RayUniquePtr<T> make_RayUniquePtr(auto&&... args) {
    return std::make_unique<T, RayPointerDeleter>(std::forward<decltype(args)>(args)...);
}

template<typename T>
RayUniquePtr<T[]> make_RayUniquePtrArray(size_t count) {
    return RayUniquePtr<T[]>(std::bit_cast<T*>(RL_MALLOC(count * sizeof(T))));
}

#if __has_include(<gsl/gsl>)
template<class T, class = std::enable_if_t<std::is_pointer<T>::value>>
using owner = T;
#else
template<typename T>
using owner = T;
#endif

template<typename T, typename D = RayPointerDeleter>
struct RayArrayHolder {
    std::unique_ptr<T, D> data{nullptr};
    size_t size{0};

    RayArrayHolder() = default;
    RayArrayHolder(owner<T*> _data, size_t _size) : data(_data), size(_size) {}

    operator std::span<T>() { return {data.get(), size}; }
    operator std::span<const T>() const { return {data.get(), size}; }

    std::span<T> AsSpan() { return {data.get(), size}; }
    std::span<const T> AsSpan() const { return {data.get(), size}; }
};

#ifdef __cpp_lib_mdspan
template<typename T, typename D = RayPointerDeleter>
struct Ray2DArrayHolder {
    using TExtents = std::extents<std::size_t, std::dynamic_extent, std::dynamic_extent>;
    using TMdspan = std::mdspan<T, TExtents>;
    using ConstTMdspan = std::mdspan<const T, TransformExtents>;

    std::unique_ptr<T, D> data{nullptr};
    size_t width{0};
    size_t height{0};

    RayArrayHolder() = default;
    RayArrayHolder(owner<T*> _data, size_t _width, size_t _height) : data(_data), width(_width), height(_height) {}

    operator TMdspan() { return TMdspan(data.get(), width, height); }
    operator ConstTMdspan() const { return ConstTMdspan(data.get(), width, height); }

    auto AsMdSpan() { return TMdspan(data.get(), width, height); }
    auto AsMdSpan() const { return ConstTMdspan(data.get(), width, height); }
};
#endif


inline static constexpr ::Image NullImage{
    .data = nullptr,
    .width = 0,
    .height = 0,
    .mipmaps = 0,
    .format = 0,
};

inline static constexpr ::Texture NullTexture{
    .id = 0,
    .width = 0,
    .height = 0,
    .mipmaps = 0,
    .format = 0,
};

inline static constexpr ::Shader NullShader{
    .id = 0,
    .locs = nullptr,
};

} // namespace raylib

#endif // RAYLIB_CPP_INCLUDE_RAYLIB_CPP_UTILS_HPP_
