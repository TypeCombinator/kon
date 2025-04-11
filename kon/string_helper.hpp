// SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef STRING_HELPER_34B723F0_16AA_49CF_9CE4_67CA9538C1FE
#define STRING_HELPER_34B723F0_16AA_49CF_9CE4_67CA9538C1FE
#include <string_view>

namespace kon {

template <char Delimiter>
struct string_splitter {
    string_splitter(const char *str, std::size_t size) noexcept {
        buf = str;
        buf_end = buf + size;
    }

    bool next(std::string_view &result) noexcept {
        const char *start = buf;
        for (; buf < buf_end; buf++) {
            if (*buf == Delimiter) {
                if (buf != start) {
                    result = std::string_view(start, buf - start);
                    return true;
                }
                start = buf + 1;
            }
        }
        if (start < buf) {
            result = std::string_view(start, buf - start);
            return true;
        }
        return false;
    }

   private:
    const char *buf;
    const char *buf_end;
};

} // namespace kon

#endif /* string_helper.hpp */