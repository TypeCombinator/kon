// SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef DEV_MEM_EF9B2540_6ED1_45B2_B355_EF3690ABC0BE
#define DEV_MEM_EF9B2540_6ED1_45B2_B355_EF3690ABC0BE
#include <cstdint>
#include <utility>

namespace kon {
class dev_mem {
   public:
    dev_mem(bool immediate = true) noexcept;
    ~dev_mem() noexcept;

    dev_mem(const dev_mem &) = delete;
    dev_mem &operator=(const dev_mem &) = delete;

    dev_mem(dev_mem &&other) noexcept {
        move_from(std::move(other));
    }

    dev_mem &operator=(dev_mem &&other) noexcept {
        move_from(std::move(other));
        return *this;
    }

    void open() noexcept;

    explicit operator bool() const noexcept {
        return m_fd >= 0;
    }

    // auto page_size = sysconf(_SC_PAGESIZE);
    // size = (size + (page_size - 1)) & (page_size - 1);
    void *map(std::uintptr_t address, std::size_t size) noexcept;
    void unmap(void *va, std::size_t size) noexcept;

   private:
    void move_from(dev_mem &&other) noexcept {
        m_fd = other.m_fd;
        other.m_fd = -1;
    }

    int m_fd;
};

} // namespace kon
#endif /* dev_mem.hpp */