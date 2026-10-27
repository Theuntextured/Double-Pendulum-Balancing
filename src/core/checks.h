#include <csignal>
#include <cstdlib>

#include "Log.h"

#if defined(_MSC_VER)
    #include <intrin.h>
#elif defined(_WIN32)
    #include <windows.h>
#endif

#if defined(__has_builtin)
    #if __has_builtin(__builtin_debugtrap)
        #define DEBUG_BREAK_HAS_BUILTIN 1
    #endif
#endif

inline void debugBreak() noexcept {
#if defined(_MSC_VER)
    __debugbreak();

#elif defined(DEBUG_BREAK_HAS_BUILTIN)
    __builtin_debugtrap();

#elif defined(_WIN32)
    ::DebugBreak();

#elif (defined(__GNUC__) || defined(__clang__)) && \
(defined(__i386__) || defined(__x86_64__))
    __asm__ volatile("int3");

#elif (defined(__GNUC__) || defined(__clang__)) && defined(__aarch64__)
    __asm__ volatile("brk #0");

#elif (defined(__GNUC__) || defined(__clang__)) && \
defined(__arm__) && defined(__ARM_ARCH) && (__ARM_ARCH >= 5)
    __asm__ volatile("bkpt #0");

#elif defined(SIGTRAP)
    std::raise(SIGTRAP);
#endif
}

#undef DEBUG_BREAK_HAS_BUILTIN

DECLARE_LOG_GROUP(Checks)

#ifdef DEBUG_MODE
#define ENSURE_BREAKS 1
#else
#define ENSURE_BREAKS 0
#endif

#define ensure(CONDITION) {\
    if (!(!!(CONDITION))) {\
        LOG(Checks, Error, "Ensure hit: ", STRINGIFY(CONDITION));\
        if constexpr (ENSURE_BREAKS) debugBreak();\
    }\
}

#define check(CONDITION, ...) {\
    if (!(!!(CONDITION))) {\
        LOG(Checks, Fatal, "Check hit: ", STRINGIFY(CONDITION));\
        debugBreak();\
    }\
}

#define ensuref(CONDITION, FORMAT, ...) {\
    if (!(!!(CONDITION))) {\
        LOGF(Checks, Error, "Ensure hit: " STRINGIFY(CONDITION) ". " FORMAT,  __VA_ARGS__);\
        if constexpr (ENSURE_BREAKS) debugBreak();\
    }\
}

#define checkf(CONDITION, ...) {\
    if (!(!!(CONDITION))) {\
    LOGF(Checks, Fatal, "Check hit: " STRINGIFY(CONDITION) ". " FORMAT,  __VA_ARGS__);\
        debugBreak();\
    }\
}
