// SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#include <kon/dev_mem.hpp>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

namespace kon {

dev_mem::dev_mem(bool immediate) noexcept {
    if (immediate) {
        m_fd = ::open("/dev/mem", O_RDWR | O_SYNC);
        return;
    }
    m_fd = -1;
}

dev_mem::~dev_mem() noexcept {
    if (m_fd >= 0) {
        close(m_fd);
        m_fd = -1;
    }
}

void dev_mem::open() noexcept {
    m_fd = ::open("/dev/mem", O_RDWR | O_SYNC);
}

void *dev_mem::map(std::uintptr_t address, std::size_t size) noexcept {
    void *va = ::mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, address);
    if (va == MAP_FAILED) {
        return nullptr;
    }
    return va;
}

void dev_mem::unmap(void *va, std::size_t size) noexcept {
    ::munmap(va, size);
}

} // namespace kon