#ifndef SHM_B2DCD76C_1F6E_4641_92FB_030FFF2B96FD
#define SHM_B2DCD76C_1F6E_4641_92FB_030FFF2B96FD

#include <string>

namespace kon {
class shm {
   public:
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
   private:
    void move_from(shm &&other) noexcept {
        mfd = other.mfd;
        memory = other.memory;
        msize = other.msize;

        other.mfd = -1;
        other.memory = nullptr;
        other.msize = 0;
    }

    int mfd;
    void *memory;
    std::size_t msize;
};
} // namespace kon
#endif /* shm.hpp */