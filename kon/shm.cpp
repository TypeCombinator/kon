// SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#include <kon/shm.hpp>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <filesystem>

namespace kon {
shm::shm(int& err, const std::string& file, std::size_t size) noexcept {
    mfd = -1;
    memory = nullptr;
    msize = 0;

    auto&& file_path = std::filesystem::path{"/dev/shm"} / file;
    struct stat st;
    bool is_file_exist = (stat(file_path.c_str(), &st) == 0);
    if (is_file_exist && (size > st.st_size)) {
        err = -1;
        return;
    }
    auto page_size = sysconf(_SC_PAGESIZE);
    if (page_size <= 0) {
        err = -2;
        return;
    }
    // Align to the page size.
    msize = (size + (page_size - 1)) & (~static_cast<std::size_t>(page_size - 1));
    int fd;
    if (is_file_exist) {
        fd = shm_open(file.c_str(), O_RDWR, 0600);
    } else {
        fd = shm_open(file.c_str(), O_CREAT | O_RDWR, 0600);
    }
    if (fd < 0) {
        err = -3;
        return;
    }
    if ((!is_file_exist) && (ftruncate(fd, msize) != 0)) {
        err = -4;
        close(fd);
        return;
    }
    auto* m = mmap(0, msize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (m == MAP_FAILED) {
        err = -5;
        close(fd);
        return;
    }
    mis_first = !is_file_exist;
    mfd = fd;
    memory = m;

    err = 0;
}

shm::~shm() noexcept {
    if (memory == nullptr) {
        munmap(memory, msize);
        memory = nullptr;
        msize = 0;
    }
    if (mfd >= 0) {
        close(mfd);
        mfd = -1;
    }
}


} // namespace kon