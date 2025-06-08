#ifndef BIO_D0F617D2_E728_44C4_8D6C_FA950B854B95
#define BIO_D0F617D2_E728_44C4_8D6C_FA950B854B95
#include <cstdint>
#include <cstring>
#include <type_traits>

namespace kon {

// Shit C++ standard!
template <typename T>
class bio {
   public:
    template <typename MT>
    MT read(MT T::*field) const noexcept {
        MT result;
        std::memcpy(
            &result,
            m_data + reinterpret_cast<size_t>(&(static_cast<T *>(nullptr)->*field)),
            sizeof(result));
        return result;
    }

    template <typename MT>
    void read_to(MT &result, MT T::*field) const noexcept {
        std::memcpy(
            &result,
            m_data + reinterpret_cast<size_t>(&(static_cast<T *>(nullptr)->*field)),
            sizeof(result));
    }

    template <typename MT>
    void write(const std::type_identity_t<MT> &value, MT T::*field) const noexcept {
        std::memcpy(
            m_data + reinterpret_cast<size_t>(&(static_cast<T *>(nullptr)->*field)),
            &value,
            sizeof(value));
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