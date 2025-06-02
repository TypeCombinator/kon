#ifndef LOG_64B080C0_927C_4ABA_ABAF_AF90D4715D49
#define LOG_64B080C0_927C_4ABA_ABAF_AF90D4715D49
#include <fmt/base.h>

namespace kon {
extern const char base16_encode_lut2[512];

struct mem_cview {
    const void* va;
    std::size_t size;
};
} // namespace kon

template <>
struct fmt::formatter<kon::mem_cview> {
    constexpr auto parse(fmt::format_parse_context& ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const kon::mem_cview& mem, FormatContext& ctx) const {
        auto* data = static_cast<const unsigned char*>(mem.va);
        const auto* data_end = data + mem.size;
        auto inserter = ctx.out();
        std::size_t line_count = 0;
        for (; data < data_end; data++) {
            if (line_count < 32u) {
                if ((line_count != 0) && (line_count & 3) == 0) {
                    *inserter++ = ' ';
                }
            } else {
                *inserter++ = '\n';
                line_count = 0;
            }
            const char* encode_pair = &kon::base16_encode_lut2[*data << 1];
            *inserter++ = encode_pair[0];
            *inserter++ = encode_pair[1];

            line_count++;
        }
        return inserter;
    }
};

#endif // log.hpp