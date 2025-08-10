// SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef SHM_B2DCD76C_1F6E_4641_92FB_030FFF2B96FD
#define SHM_B2DCD76C_1F6E_4641_92FB_030FFF2B96FD

#include <string>

namespace kon {
class shm {
   public:
    shm() noexcept {
        mis_first = false;
        mfd = -1;
        memory = nullptr;
        msize = 0;
    }

    shm(int &err, const std::string &file, std::size_t size) noexcept;
    ~shm() noexcept;

    shm(const shm &) = delete;
    shm &operator=(const shm &) = delete;

    shm(shm &&other) noexcept {
        move_from(std::move(other));
    }

    shm &operator=(shm &&other) noexcept {
        move_from(std::move(other));
        return *this;
    }

    void *data() noexcept {
        return memory;
    }

    std::size_t size() const noexcept {
        return msize;
    }

    bool is_first() const noexcept {
        return mis_first;
    }
   private:
    void move_from(shm &&other) noexcept {
        mis_first = other.mis_first;
        mfd = other.mfd;
        memory = other.memory;
        msize = other.msize;

        other.mis_first = false;
        other.mfd = -1;
        other.memory = nullptr;
        other.msize = 0;
    }

    bool mis_first;
    int mfd;
    void *memory;
    std::size_t msize;
};
} // namespace kon
#endif /* shm.hpp */