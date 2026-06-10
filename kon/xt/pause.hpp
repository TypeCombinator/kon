#ifndef RT_PAUSE_CB0E1BA1_CC8B_4D82_B5A3_994967B25341
#define RT_PAUSE_CB0E1BA1_CC8B_4D82_B5A3_994967B25341
#include <kon/xt/attributes.hpp>

namespace kon::rt {
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
KON_ATTR_ALWAYS_INLINE static void pause() noexcept {
    __builtin_ia32_pause();
}
#elif defined(__arm__) || defined(__aarch64__) || defined(_M_ARM64)
KON_ATTR_ALWAYS_INLINE static void pause() noexcept {
    #if (                                                                                          \
        defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK__)          \
        || defined(__ARM_ARCH_6T2__) || defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__)        \
        || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)        \
        || defined(__ARM_ARCH_8A__) || defined(__aarch64__))
    asm volatile("yield" ::: "memory");
    #elif defined(_M_ARM64)
    __yield();
    #else
    asm volatile("nop" ::: "memory");
    #endif
}
#else
KON_ATTR_ALWAYS_INLINE static void pause() noexcept {
}
#endif
} // namespace kon::rt
#endif // pause.hpp