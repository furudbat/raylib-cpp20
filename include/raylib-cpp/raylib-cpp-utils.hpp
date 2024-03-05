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
#endif

#ifndef GETTERSETTER
/**
 * A utility to build get and set methods on top of a property.
 *
 * @param type The type of the property.
 * @param method The human-readable name for the method.
 * @param name The machine-readable name of the property.
 */
#define GETTERSETTER(type, method, name) \
    /** Retrieves the name value for the object. @return The name value of the object. */       \
    constexpr type Get##method() & { return name; }                                             \
    constexpr type Get##method() const & { return name; }                                       \
    constexpr type Get##method() && { return std::move(name); }                                 \
    /** Sets the name value for the object. @param value The value of which to set name to. */  \
    constexpr void Set##method(type value) { name = value; }
#endif
#ifndef GETTER
#define GETTER(type, method, name) \
    /** Retrieves the name value for the object. @return The name value of the object. */       \
    constexpr type Get##method() & { return name; }                                             \
    constexpr type Get##method() const & { return name; }                                       \
    constexpr type Get##method() && { return std::move(name); }
#endif
#ifndef CONST_GETTER
#define CONST_GETTER(type, method, name) \
    /** Retrieves the name value for the object. @return The name value of the object. */       \
    constexpr const type Get##method() const { return name; }
#endif
#ifndef CONST_GETTERSETTER
#define CONST_GETTERSETTER(type, method, name) \
    /** Retrieves the name value for the object. @return The name value of the object. */       \
    constexpr const type Get##method() const & { return name; }                                 \
    constexpr type Get##method() && { return std::move(name); }                                 \
    /** Sets the name value for the object. @param value The value of which to set name to. */  \
    constexpr void Set##method(type value) { name = value; }
#endif
#ifndef SPAN_GETTER
#define SPAN_GETTER(type, method, name, size) \
    /** Retrieves the name value for the object. @return The name value of the object. */               \
    constexpr std::span<const type> Get##method() const { return {name, static_cast<size_t>(size)}; }   \
    constexpr std::span<type> Get##method() { return {name, static_cast<size_t>(size)}; }
#endif

#ifndef COMPOSITION_METHODE_CALL
#define COMPOSITION_METHODE_CALL(method, name) \
    constexpr auto method(auto&&... args) { return name.method(std::forward<decltype(args)>(args)...); }
#endif
#ifndef CONST_COMPOSITION_METHODE_CALL
#define CONST_COMPOSITION_METHODE_CALL(method, name) \
    constexpr auto method(auto&&... args) const { return name.method(std::forward<decltype(args)>(args)...); }
#endif
#ifndef COMPOSITION_METHODE_CALL_RETURN_THIS
#define COMPOSITION_METHODE_CALL_RETURN_THIS(method, name) \
    constexpr auto& method(auto&&... args) { name.method(std::forward<decltype(args)>(args)...); return *this; }
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
#define RAYLIB_CPP_RETURN_EXPECTED()
#define RAYLIB_CPP_THROWS noexcept(false)
#else
#define RAYLIB_CPP_RETURN_UNEXPECTED_OR_THROW(Error) std::abort();
#define RAYLIB_CPP_RETURN_EXPECTED_VALUE(Value) return Value;
#define RAYLIB_CPP_RETURN_EXPECTED()
#define RAYLIB_CPP_THROWS
#endif
#endif

namespace raylib {

struct RayPointerDeleter {
    template<typename T>
    constexpr void operator()(T *arg) const {
        RL_FREE(arg);
    }
};

template<typename T>
using RayUniquePtr = std::unique_ptr<T, RayPointerDeleter>;

#if __has_include(<gsl/gsl>)
template <typename T>
using owner = gsl::owner<T>;
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

    operator std::span<T>() {
        return {data.get(), size};
    }
    operator std::span<const T>() const {
        return {data.get(), size};
    }

    std::span<T> AsSpan() { return {data.get(), size}; }
    std::span<const T> AsSpan() const { return {data.get(), size}; }
};


inline static constexpr ::Image NullImage {
        .data = nullptr,
        .width = 0,
        .height = 0,
        .mipmaps = 0,
        .format = 0,
};

inline static constexpr ::Texture NullTexture {
        .id = 0,
        .width = 0,
        .height = 0,
        .mipmaps = 0,
        .format = 0,
};

inline static constexpr ::Shader NullShader {
        .id = 0,
        .locs = nullptr,
};

}

#endif  // RAYLIB_CPP_INCLUDE_RAYLIB_CPP_UTILS_HPP_
