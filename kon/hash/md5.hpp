// SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef HASH_MD5_EF91EA4D_C9D5_4146_83F7_422D04457955
#define HASH_MD5_EF91EA4D_C9D5_4146_83F7_422D04457955
// References:
// [0]: http://www.ietf.org/rfc/rfc1321.txt
#include <string>
#include <cstdint>

namespace kon {

class md5_context {
   public:
    void start() noexcept;
    void update(const unsigned char *input, size_t ilen) noexcept;
    void finish(unsigned char output[16]) noexcept;
   private:
    void process(const unsigned char data[64]) noexcept;

    uint32_t m_total[2];        // number of bytes processed
    uint32_t m_state[4];        // intermediate digest state
    unsigned char m_buffer[64]; // data block being processed
};

static inline void md5(const unsigned char *input, size_t ilen, unsigned char output[16]) noexcept {
    md5_context ctx;

    ctx.start();
    ctx.update(input, ilen);
    ctx.finish(output);
}

int file_md5(const std::string &file_name, unsigned char output[16]) noexcept;

} // namespace kon
#endif // md5.hpp