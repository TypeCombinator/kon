// SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef DBUF_57990DD2_DCDD_4B4F_BF64_B850D2936EFB
#define DBUF_57990DD2_DCDD_4B4F_BF64_B850D2936EFB


#include <cstdint>
#include <cstddef>
#include <type_traits>

namespace kon {

// Device buffer view.
class dbuf {
   public:
    void init(uint8_t *va, size_t iova, uint32_t headroom, uint32_t size) noexcept {
        buf_va = va;
        buf_iova = iova;
        buf_len = size;
        data_len = 0;
        data_off = (headroom < size) ? headroom : size;
    }

    void reset(uint32_t headroom, uint32_t data_length) noexcept {
        data_len = data_length;
        data_off = (headroom < buf_len) ? headroom : buf_len;
    }

    size_t iova(void) noexcept {
        return buf_iova;
    }

    template <typename T = uint8_t[], typename ET = std::remove_extent_t<T>>
        requires(std::is_trivial_v<std::remove_all_extents_t<T>>)
    ET *append(uint32_t number = 1) noexcept {
        uint32_t size = number * sizeof(ET);
        if (size > (uint32_t) (buf_len - data_off - data_len)) [[unlikely]] {
            return nullptr;
        }
        uint8_t *tail;
        tail = buf_va + data_off + data_len;
        data_len += size;
        return new (tail) ET[number];
    }

    template <typename T = uint8_t[], typename ET = std::remove_extent_t<T>>
        requires(std::is_trivial_v<std::remove_all_extents_t<T>>)
    ET *preppend(uint32_t number = 1) noexcept {
        uint32_t size = number * sizeof(ET);
        if (size > data_off) [[unlikely]] {
            return nullptr;
        }
        data_off -= size;
        data_len += size;
        return new (buf_va + data_off) ET[number];
    }

    uint8_t *adjust(uint32_t size) noexcept {
        if (size > data_len) [[unlikely]] {
            return nullptr;
        }
        data_len -= size;
        data_off += size;
        return buf_va + data_off;
    }

    template <typename T = uint8_t[], typename ET = std::remove_extent_t<T>>
        requires(std::is_trivial_v<std::remove_all_extents_t<T>>)
    ET *read(uint32_t number = 1) noexcept {
        uint32_t size = number * sizeof(ET);
        if (size > data_len) [[unlikely]] {
            return nullptr;
        }
        uint8_t *head;
        head = buf_va + data_off;
        data_len -= size;
        data_off += size;
        return new (head) ET[number];
    }

    uint32_t data_length() noexcept {
        return data_len;
    }

    uint8_t *data() noexcept {
        return buf_va + data_off;
    }

    size_t data_iova() noexcept {
        return buf_iova + data_off;
    }
   private:
    uint8_t *buf_va;
    size_t buf_iova;
    uint32_t buf_len;
    uint32_t data_len;
    uint32_t data_off;
};

} // namespace kon

#endif /* dbuf.hpp */