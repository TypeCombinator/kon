#ifndef XT_ATTRIBUTES_197F11CD_E55A_4F3C_9E27_31C7890648BC
#define XT_ATTRIBUTES_197F11CD_E55A_4F3C_9E27_31C7890648BC


#if __has_attribute(always_inline)
    #define KON_ATTR_ALWAYS_INLINE [[gnu::always_inline]] inline
#elif defined(_MSC_VER)
    #define KON_ATTR_ALWAYS_INLINE [[msvc::forceinline]] inline
#else
    #define KON_ATTR_ALWAYS_INLINE inline
#endif

#if defined(_MSC_VER) and not defined(__clang__)
    #define KON_ATTR_NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#elif defined(_MSC_VER) and defined(__clang__) and (__clang_major__ >= 18)
    #define KON_ATTR_NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#else
    #define KON_ATTR_NO_UNIQUE_ADDRESS [[no_unique_address]]
#endif


#if defined(__clang__)
    #define KON_ATTR_ASSUME(cond) __builtin_assume(cond)
#elif defined(_MSC_VER)
    #define KON_ATTR_ASSUME(cond) __assume(cond)
#elif defined(__GNUC__) && (__GNUC__ > 12)
    #define KON_ATTR_ASSUME(cond) __builtin_assume(cond)
#else
    #define KON_ATTR_ASSUME(cond)                                                                  \
        do {                                                                                       \
            if (!(cond))                                                                           \
                __builtin_unreachable();                                                           \
        } while (0)
#endif

#define KON_DISALLOW_COPY(_name_)                                                                  \
    _name_(const _name_ &) = delete;                                                               \
    _name_ &operator=(const _name_ &) = delete

#define KON_DISALLOW_MOVE(_name_)                                                                  \
    _name_(_name_ &&) = delete;                                                                    \
    _name_ &operator=(_name_ &&) = delete

#endif // attr.hpp