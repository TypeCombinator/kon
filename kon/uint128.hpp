#ifndef UINT128_C20EF4A2_A2ED_4EE6_91C4_7914D0A5B00C
#define UINT128_C20EF4A2_A2ED_4EE6_91C4_7914D0A5B00C
#include <cstdint>
// ============================================================================
//  CASE 1: GCC / Clang / LLVM with native unsigned __int128
// ============================================================================
#if defined(__SIZEOF_INT128__)

namespace kon {
using uint128_t = __uint128_t;

// No operator overloads here — GCC/Clang already support them natively.

inline std::uint64_t lo128(uint128_t v) {
    return (std::uint64_t) v;
}

inline std::uint64_t hi128(uint128_t v) {
    return (std::uint64_t) (v >> 64);
}
} // namespace kon
#else


// ============================================================================
//  CASE 2: MSVC — implement uint128_t manually
// ============================================================================
namespace tpx {

struct uint128_t {
    std::uint64_t lo;
    std::uint64_t hi;

    uint128_t() = default;

    // unsigned
    uint128_t(std::uint64_t v)
        : lo(v)
        , hi(0) {
    }

    uint128_t(uint32_t v)
        : lo(v)
        , hi(0) {
    }

    // signed literal support (needed!)
    uint128_t(int v)
        : lo((std::uint64_t) (uint32_t) v)
        , hi(0) {
    }

    uint128_t(int64_t v)
        : lo((std::uint64_t) v)
        , hi(0) {
    }

    // explicit hi/lo constructor
    uint128_t(std::uint64_t hi_, std::uint64_t lo_)
        : lo(lo_)
        , hi(hi_) {
    }

    explicit operator std::uint64_t() const {
        return lo;
    }
};

    // ============================================================================
    //  Backend for 64×64 → 128 multiply
    // ============================================================================
    #if defined(_MSC_VER) && defined(_M_X64)

        #include <intrin.h>

inline uint128_t u128_mul64(std::uint64_t a, std::uint64_t b) {
    uint128_t r;
    r.lo = _umul128(a, b, &r.hi);
    return r;
}

    #elif defined(_MSC_VER) && defined(_M_ARM64)

        #include <intrin.h>

// ARM64 MSVC: hi via __umulh, lo via normal mul
inline uint128_t u128_mul64(std::uint64_t a, std::uint64_t b) {
    uint128_t r;
    r.hi = __umulh(a, b);
    r.lo = a * b;
    return r;
}

    #else
        #error "MSVC platform missing 128-bit multiply support."
    #endif


inline uint128_t operator*(uint128_t a, std::uint64_t b) {
    uint128_t r = u128_mul64(a.lo, b);
    r.hi += a.hi * b;
    return r;
}

inline uint128_t operator*(std::uint64_t a, uint128_t b) {
    uint128_t r = u128_mul64(a, b.lo);
    r.hi += b.hi * a;
    return r;
}

// ============================================================================
//  Right-shift operator: return 64-bit high part when >> 64
// ============================================================================
inline std::uint64_t operator>>(const uint128_t& v, unsigned shift) {
    if (shift == 0) {
        return v.lo;
    }
    if (shift < 64) {
        return (v.lo >> shift) | (v.hi << (64 - shift));
    }
    if (shift == 64) {
        return v.hi;
    }
    if (shift < 128) {
        return v.hi >> (shift - 64);
    }
    return 0;
}

inline std::uint64_t lo128(const uint128_t& v) {
    return v.lo;
}

inline std::uint64_t hi128(const uint128_t& v) {
    return v.hi;
}
} // namespace tpx
#endif

#endif /* uint128.hpp */