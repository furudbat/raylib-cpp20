#ifndef RAYLIB_CPP_INCLUDE_RAYLIBERROR_HPP_
#define RAYLIB_CPP_INCLUDE_RAYLIBERROR_HPP_


#include "raylib.hpp"

#include <string>
#include <optional>

namespace raylib {

enum class RaylibErrorCode {
    None = 0,
    Unknown,
};

/**
 * Exception used for most raylib-related exceptions.
 */
class RaylibError {
 public:
    /**
     * Construct a runtime exception with the given message.
     *
     * @param message The message to provide for the exception.
     */
    explicit RaylibError(const std::string& msg) : message(msg) {
        // Nothing
    }

    RaylibError(RaylibErrorCode _code, const std::string& msg) : code(_code), message(msg) {
        // Nothing
    }

    /**
     * Outputs the exception message to TraceLog().
     *
     * @param logLevel The output status to use when outputing.
     */
    void TraceLog(int logLevel = LOG_ERROR) {
        ::TraceLog(logLevel, message.c_str());
    }

    const std::string& what() const noexcept { return message; }
    RaylibErrorCode GetCode() const noexcept { return code; }

    const std::string& GetMessage() const & noexcept { return message; }
    std::string GetMessage() && noexcept { return std::move(message); }

private:
    RaylibErrorCode code{RaylibErrorCode::Unknown};
    std::string message;
};

template<class R>
using RaylibExpectedError = expected<R, RaylibError>;

#ifdef RAYLIB_CPP_EXPECTED
#define RAYLIB_CPP_EXPECTED_STATIC_RESULT(R) [[nodiscard]] static RaylibExpectedError<R>
#define RAYLIB_CPP_EXPECTED_RESULT(R) RaylibExpectedError<R>
#define RAYLIB_CPP_EXPECTED_RESULT_VOID RaylibExpectedError<void>
#else
#define RAYLIB_CPP_EXPECTED_STATIC_RESULT(R) [[nodiscard]] static R
#define RAYLIB_CPP_EXPECTED_RESULT(R) R
#define RAYLIB_CPP_EXPECTED_RESULT_VOID void
#endif

}  // namespace raylib

using RRaylibError = raylib::RaylibError;

#endif  // RAYLIB_CPP_INCLUDE_RAYLIBEXCEPTION_HPP_
