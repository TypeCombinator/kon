// SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef DBUF_57990DD2_DCDD_4B4F_BF64_B850D2936EFB
#define DBUF_57990DD2_DCDD_4B4F_BF64_B850D2936EFB


#include <cstdint>
#include <cstddef>
#include <type_traits>

namespace kon {

// Device buffer view, it's easy to use, although there are some UBs.
class dbuf {
   public:
    dbuf() noexcept
        : buf_va{nullptr}
        , buf_iova{0}
        , buf_len{0}
        , data_len{0}
        , data_off{0} {
    }

    dbuf(std::uint8_t *va, std::uintptr_t iova, std::uint32_t headroom, std::uint32_t size) noexcept
        : buf_va{va}
        , buf_iova{iova}
        , buf_len{size}
        , data_len{0}
        , data_off{(headroom < size) ? headroom : size} {
    }

    void init(
        std::uint8_t *va,
        std::uintptr_t iova,
        std::uint32_t headroom,
        std::uint32_t size) noexcept {
        buf_va = va;
        buf_iova = iova;
        buf_len = size;
        data_len = 0;
        data_off = (headroom < size) ? headroom : size;
    }

    struct anchor {
        std::uint32_t length;
        std::uint32_t offset;
    };

    anchor store() noexcept {
        return anchor{data_len, data_off};
    }

    void restore(const anchor &point) noexcept {
        data_len = point.length;
        data_off = point.offset;
    }

    void reset(std::uint32_t headroom, std::uint32_t data_length) noexcept {
        data_len = data_length;
        data_off = (headroom < buf_len) ? headroom : buf_len;
    }

    std::uint8_t *va() const noexcept {
        return buf_va;
    }

    std::uintptr_t iova() const noexcept {
        return buf_iova;
    }

    std::uint32_t capacity() const noexcept {
        return buf_len;
    }

    std::uint32_t headroom() const noexcept {
        return data_off;
    }

    template <typename T = std::uint8_t[], typename ET = std::remove_extent_t<T>>
        requires(std::is_trivial_v<std::remove_all_extents_t<T>>)
    ET *append(uint32_t number = 1) noexcept {
        std::uint32_t size = number * sizeof(ET);
        std::uint32_t offset = data_off + data_len;
        if (size > (std::uint32_t) (buf_len - offset)) [[unlikely]] {
            return nullptr;
        }
        std::uint8_t *tail = buf_va + offset;
        data_len += size;
        return new (tail) ET[number];
    }

    std::uint8_t *append_rest_begin(std::uint32_t &rest_size) noexcept {
        std::uint32_t offset = data_off + data_len;
        rest_size = buf_len - offset;
        return buf_va + offset;
    }

    // Notice: size <= rest_size must be ensured, otherwise it's UB!
    void append_rest_end(std::uint32_t size) noexcept {
        data_len += size;
    }

    template <typename T = std::uint8_t[], typename ET = std::remove_extent_t<T>>
        requires(std::is_trivial_v<std::remove_all_extents_t<T>>)
    ET *prepend(std::uint32_t number = 1) noexcept {
        std::uint32_t size = number * sizeof(ET);
        if (size > data_off) [[unlikely]] {
            return nullptr;
        }
        data_off -= size;
        data_len += size;
        return new (buf_va + data_off) ET[number];
    }

    std::uint8_t *adjust(std::uint32_t size) noexcept {
        if (size > data_len) [[unlikely]] {
            return nullptr;
        }
        data_len -= size;
        data_off += size;
        return buf_va + data_off;
    }

    template <typename T = std::uint8_t[], typename ET = std::remove_extent_t<T>>
        requires(std::is_trivial_v<std::remove_all_extents_t<T>>)
    ET *read(std::uint32_t number = 1) noexcept {
        std::uint32_t size = number * sizeof(ET);
        if (size > data_len) [[unlikely]] {
            return nullptr;
        }
        std::uint8_t *head;
        head = buf_va + data_off;
        data_len -= size;
        data_off += size;
        return new (head) ET[number];
    }

    std::uint32_t data_length() const noexcept {
        return data_len;
    }

    std::uint8_t *data() const noexcept {
        return buf_va + data_off;
    }

    std::uintptr_t data_iova() const noexcept {
        return buf_iova + data_off;
    }
   private:
    std::uint8_t *buf_va;
    std::uintptr_t buf_iova;
    std::uint32_t buf_len;
    std::uint32_t data_len;
    std::uint32_t data_off;
    std::uint8_t m_padding[4];
   public:
    static constexpr std::size_t padding_size = sizeof(m_padding);

    std::uint8_t *padding() noexcept {
        return m_padding;
    }
};

} // namespace kon

#endif /* dbuf.hpp */