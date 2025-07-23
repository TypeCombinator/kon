// SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef BIO_D0F617D2_E728_44C4_8D6C_FA950B854B95
#define BIO_D0F617D2_E728_44C4_8D6C_FA950B854B95
#include <cstdint>
#include <cstring>
#include <type_traits>
#include <bit>
#include <kon/bit.hpp>

namespace kon {

template <typename T, bool is_target_big_endian = false>
class bio {
    static constexpr bool need_swap = is_target_big_endian
                                   && (std::endian::native == std::endian::little);
   public:
    template <typename MT>
    MT read(MT T::*field) const noexcept {
        MT v;
        std::memcpy(
            &v, m_data + reinterpret_cast<size_t>(&(static_cast<T *>(nullptr)->*field)), sizeof(v));
        if constexpr (need_swap) {
            return byteswap(v);
        } else {
            return v;
        }
    }

    template <typename MT>
    void read_to(MT &result, MT T::*field) const noexcept {
        if constexpr (need_swap) {
            MT v;
            std::memcpy(
                &v,
                m_data + reinterpret_cast<size_t>(&(static_cast<T *>(nullptr)->*field)),
                sizeof(result));
            result = byteswap(v);
        } else {
            std::memcpy(
                &result,
                m_data + reinterpret_cast<size_t>(&(static_cast<T *>(nullptr)->*field)),
                sizeof(result));
        }
    }

    template <typename MT>
    void write(const std::type_identity_t<MT> &value, MT T::*field) const noexcept {
        if constexpr (need_swap) {
            MT v;
            v = byteswap(value);
            std::memcpy(
                m_data + reinterpret_cast<size_t>(&(static_cast<T *>(nullptr)->*field)),
                &v,
                sizeof(value));
        } else {
            std::memcpy(
                m_data + reinterpret_cast<size_t>(&(static_cast<T *>(nullptr)->*field)),
                &value,
                sizeof(value));
        }
    }

    using type = T;

    uint8_t *m_data;
};

} // namespace kon

#define KON_BIO_READ(_bio_, _field_) (_bio_).read(&decltype(_bio_)::type::_field_)

#define KON_BIO_READ_TO(_bio_, _result_, _field_)                                                  \
    (_bio_).read_to(_result_, &decltype(_bio_)::type::_field_)

#define KON_BIO_WRITE(_bio_, _value_, _field_)                                                     \
    (_bio_).write(_value_, &decltype(_bio_)::type::_field_)

#endif // bio.hpp