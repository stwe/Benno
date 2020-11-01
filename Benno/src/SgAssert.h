#pragma once

#include "Log.h"

#if defined(_WIN64)
    #define SG_DEBUG_BREAK __debugbreak()
#elif defined(__linux__) && defined(__GNUC__) && (__GNUC__ >= 7)
    #include <signal.h>
    #define SG_DEBUG_BREAK raise(SIGTRAP)
#else
    #error Unsupported platform or compiler!
#endif

#ifdef SG_DEBUG_BUILD
    #define SG_ENABLE_ASSERTS
#endif

#ifdef SG_ENABLE_ASSERTS

    inline auto SG_ASSERT = [](auto&& t_expr, const std::string_view t_str) -> void
    {
        if (!(t_expr))
        {
            sg::Log::SG_LOG_ERROR("Assertion failed: {}", t_str);
            SG_DEBUG_BREAK;
        }
    };

#else

    inline auto SG_ASSERT = [](auto&& t_expr, const std::string_view t_str) -> void
    {};

#endif
