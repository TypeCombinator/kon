// SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#include <cstring>
#include <kon/hash/md5.hpp>

#define MD5_PUT_UINT32_LE(_value_, _data_, _offset_)                                               \
    std::memcpy((_data_) + (_offset_), &(_value_), 4)

namespace kon {
void md5_context::start() noexcept {
    m_total[0] = 0;
    m_total[1] = 0;

    m_state[0] = 0x67452301;
    m_state[1] = 0xEFCDAB89;
    m_state[2] = 0x98BADCFE;
    m_state[3] = 0x10325476;
}

void md5_context::process(const unsigned char data[64]) noexcept {
    struct {
        uint32_t X[16], A, B, C, D;
    } local;

    // Need little endian!!!
    std::memcpy(local.X, data, sizeof(local.X));

#define S(x, n) (((x) << (n)) | (((x) & 0xFFFFFFFF) >> (32 - (n))))

#define P(a, b, c, d, k, s, t)                                                                     \
    do {                                                                                           \
        (a) += F((b), (c), (d)) + local.X[(k)] + (t);                                              \
        (a) = S((a), (s)) + (b);                                                                   \
    } while (0)

    local.A = m_state[0];
    local.B = m_state[1];
    local.C = m_state[2];
    local.D = m_state[3];

#define F(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))

    P(local.A, local.B, local.C, local.D, 0, 7, 0xD76AA478);
    P(local.D, local.A, local.B, local.C, 1, 12, 0xE8C7B756);
    P(local.C, local.D, local.A, local.B, 2, 17, 0x242070DB);
    P(local.B, local.C, local.D, local.A, 3, 22, 0xC1BDCEEE);
    P(local.A, local.B, local.C, local.D, 4, 7, 0xF57C0FAF);
    P(local.D, local.A, local.B, local.C, 5, 12, 0x4787C62A);
    P(local.C, local.D, local.A, local.B, 6, 17, 0xA8304613);
    P(local.B, local.C, local.D, local.A, 7, 22, 0xFD469501);
    P(local.A, local.B, local.C, local.D, 8, 7, 0x698098D8);
    P(local.D, local.A, local.B, local.C, 9, 12, 0x8B44F7AF);
    P(local.C, local.D, local.A, local.B, 10, 17, 0xFFFF5BB1);
    P(local.B, local.C, local.D, local.A, 11, 22, 0x895CD7BE);
    P(local.A, local.B, local.C, local.D, 12, 7, 0x6B901122);
    P(local.D, local.A, local.B, local.C, 13, 12, 0xFD987193);
    P(local.C, local.D, local.A, local.B, 14, 17, 0xA679438E);
    P(local.B, local.C, local.D, local.A, 15, 22, 0x49B40821);

#undef F

#define F(x, y, z) ((y) ^ ((z) & ((x) ^ (y))))

    P(local.A, local.B, local.C, local.D, 1, 5, 0xF61E2562);
    P(local.D, local.A, local.B, local.C, 6, 9, 0xC040B340);
    P(local.C, local.D, local.A, local.B, 11, 14, 0x265E5A51);
    P(local.B, local.C, local.D, local.A, 0, 20, 0xE9B6C7AA);
    P(local.A, local.B, local.C, local.D, 5, 5, 0xD62F105D);
    P(local.D, local.A, local.B, local.C, 10, 9, 0x02441453);
    P(local.C, local.D, local.A, local.B, 15, 14, 0xD8A1E681);
    P(local.B, local.C, local.D, local.A, 4, 20, 0xE7D3FBC8);
    P(local.A, local.B, local.C, local.D, 9, 5, 0x21E1CDE6);
    P(local.D, local.A, local.B, local.C, 14, 9, 0xC33707D6);
    P(local.C, local.D, local.A, local.B, 3, 14, 0xF4D50D87);
    P(local.B, local.C, local.D, local.A, 8, 20, 0x455A14ED);
    P(local.A, local.B, local.C, local.D, 13, 5, 0xA9E3E905);
    P(local.D, local.A, local.B, local.C, 2, 9, 0xFCEFA3F8);
    P(local.C, local.D, local.A, local.B, 7, 14, 0x676F02D9);
    P(local.B, local.C, local.D, local.A, 12, 20, 0x8D2A4C8A);

#undef F

#define F(x, y, z) ((x) ^ (y) ^ (z))

    P(local.A, local.B, local.C, local.D, 5, 4, 0xFFFA3942);
    P(local.D, local.A, local.B, local.C, 8, 11, 0x8771F681);
    P(local.C, local.D, local.A, local.B, 11, 16, 0x6D9D6122);
    P(local.B, local.C, local.D, local.A, 14, 23, 0xFDE5380C);
    P(local.A, local.B, local.C, local.D, 1, 4, 0xA4BEEA44);
    P(local.D, local.A, local.B, local.C, 4, 11, 0x4BDECFA9);
    P(local.C, local.D, local.A, local.B, 7, 16, 0xF6BB4B60);
    P(local.B, local.C, local.D, local.A, 10, 23, 0xBEBFBC70);
    P(local.A, local.B, local.C, local.D, 13, 4, 0x289B7EC6);
    P(local.D, local.A, local.B, local.C, 0, 11, 0xEAA127FA);
    P(local.C, local.D, local.A, local.B, 3, 16, 0xD4EF3085);
    P(local.B, local.C, local.D, local.A, 6, 23, 0x04881D05);
    P(local.A, local.B, local.C, local.D, 9, 4, 0xD9D4D039);
    P(local.D, local.A, local.B, local.C, 12, 11, 0xE6DB99E5);
    P(local.C, local.D, local.A, local.B, 15, 16, 0x1FA27CF8);
    P(local.B, local.C, local.D, local.A, 2, 23, 0xC4AC5665);

#undef F

#define F(x, y, z) ((y) ^ ((x) | ~(z)))

    P(local.A, local.B, local.C, local.D, 0, 6, 0xF4292244);
    P(local.D, local.A, local.B, local.C, 7, 10, 0x432AFF97);
    P(local.C, local.D, local.A, local.B, 14, 15, 0xAB9423A7);
    P(local.B, local.C, local.D, local.A, 5, 21, 0xFC93A039);
    P(local.A, local.B, local.C, local.D, 12, 6, 0x655B59C3);
    P(local.D, local.A, local.B, local.C, 3, 10, 0x8F0CCC92);
    P(local.C, local.D, local.A, local.B, 10, 15, 0xFFEFF47D);
    P(local.B, local.C, local.D, local.A, 1, 21, 0x85845DD1);
    P(local.A, local.B, local.C, local.D, 8, 6, 0x6FA87E4F);
    P(local.D, local.A, local.B, local.C, 15, 10, 0xFE2CE6E0);
    P(local.C, local.D, local.A, local.B, 6, 15, 0xA3014314);
    P(local.B, local.C, local.D, local.A, 13, 21, 0x4E0811A1);
    P(local.A, local.B, local.C, local.D, 4, 6, 0xF7537E82);
    P(local.D, local.A, local.B, local.C, 11, 10, 0xBD3AF235);
    P(local.C, local.D, local.A, local.B, 2, 15, 0x2AD7D2BB);
    P(local.B, local.C, local.D, local.A, 9, 21, 0xEB86D391);

#undef F

    m_state[0] += local.A;
    m_state[1] += local.B;
    m_state[2] += local.C;
    m_state[3] += local.D;
}

void md5_context::update(const unsigned char *input, size_t ilen) noexcept {
    size_t fill;
    uint32_t left;

    if (ilen == 0) {
        return;
    }

    left = m_total[0] & 0x3F;
    fill = 64 - left;

    m_total[0] += (uint32_t) ilen;
    m_total[0] &= 0xFFFFFFFF;

    if (m_total[0] < (uint32_t) ilen) {
        m_total[1]++;
    }

    if (left && ilen >= fill) {
        std::memcpy(m_buffer + left, input, fill);
        process(m_buffer);

        input += fill;
        ilen -= fill;
        left = 0;
    }

    while (ilen >= 64) {
        process(input);

        input += 64;
        ilen -= 64;
    }

    if (ilen > 0) {
        std::memcpy(m_buffer + left, input, ilen);
    }
}

void md5_context::finish(unsigned char output[16]) noexcept {
    uint32_t used;
    uint32_t high, low;
    // Add padding: 0x80 then 0x00 until 8 bytes remain for the length
    used = m_total[0] & 0x3F;

    m_buffer[used++] = 0x80;

    if (used <= 56) {
        // Enough room for padding + length in current block
        std::memset(m_buffer + used, 0, 56 - used);
    } else {
        // We'll need an extra block
        std::memset(m_buffer + used, 0, 64 - used);

        process(m_buffer);

        std::memset(m_buffer, 0, 56);
    }

    // Add message length
    high = (m_total[0] >> 29) | (m_total[1] << 3);
    low = (m_total[0] << 3);

    MD5_PUT_UINT32_LE(low, m_buffer, 56);
    MD5_PUT_UINT32_LE(high, m_buffer, 60);

    process(m_buffer);

    // Output final state
    MD5_PUT_UINT32_LE(m_state[0], output, 0);
    MD5_PUT_UINT32_LE(m_state[1], output, 4);
    MD5_PUT_UINT32_LE(m_state[2], output, 8);
    MD5_PUT_UINT32_LE(m_state[3], output, 12);
}

int file_md5(const std::string &file_name, unsigned char output[16]) noexcept {
    constexpr std::size_t block_size = 32768;

    auto stream = std::fopen(file_name.c_str(), "rb");
    if (stream == nullptr) {
        return -1;
    }
    auto buffer = new (std::nothrow) uint8_t[block_size + 72];
    if (buffer == nullptr) {
        std::fclose(stream);
        return -1;
    }

    md5_context ctx;
    ctx.start();
    size_t sum;
    // Iterate over full file contents.
    while (1) {
        // We read the file in blocks of block_size bytes.  One call of the computation function
        // processes the whole buffer so that with the next round of the loop another block can be
        // read.
        sum = 0;
        // Read block.  Take care for partial reads.
        while (1) {
            // Either process a partial fread() from this loop, or the fread() in afalg_stream may
            // have gotten EOF. We need to avoid a subsequent fread() as EOF may not be sticky. For
            // details of such systems, see: https://sourceware.org/bugzilla/show_bug.cgi?id=1190
            if (std::feof(stream)) {
                goto lb_process_partial_block;
            }
            size_t n = std::fread(buffer + sum, 1, block_size - sum, stream);
            sum += n;
            if (sum == block_size) {
                break;
            }
            if (n == 0) {
                // Check for the error flag IFF N == 0, so that we don't exit the loop after a
                // partial read due to EAGAIN or EWOULDBLOCK.
                if (std::ferror(stream)) {
                    delete[] buffer;
                    std::fclose(stream);
                    return 1;
                }
                goto lb_process_partial_block;
            }
        }
        // Process buffer with block_size bytes. Note that block_size % 64 == 0
        ctx.update(buffer, block_size);
    }
lb_process_partial_block:
    // Process any remaining bytes.
    if (sum > 0) {
        ctx.update(buffer, sum);
    }
    // Construct result in desired memory.
    ctx.finish(output);

    delete[] buffer;
    std::fclose(stream);
    return 0;
}

} // namespace kon