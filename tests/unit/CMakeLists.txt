add_executable(kon_tests
    hash/md5.cpp
    base10.cpp
    base16.cpp
    bio.cpp
    bit.cpp
    bitset.cpp
    conv.cpp
    dbuf.cpp
    file_helper.cpp
    inerting.cpp
    scope.cpp
    shm.cpp
    spin_lock.cpp
    string_helper.cpp
    utility.cpp
    vlm_ring.cpp
)

target_link_libraries(kon_tests PRIVATE
    Catch2::Catch2WithMain
    kon
)