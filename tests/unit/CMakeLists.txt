add_executable(kon_tests
    base10.cpp
    base16.cpp
    bit.cpp
    bitset.cpp
    conv.cpp
    dbuf.cpp
    file_helper.cpp
    maybe.cpp
    shm.cpp
    string_helper.cpp
    vlm_ring.cpp
)

target_link_libraries(kon_tests PRIVATE
    Catch2::Catch2WithMain
    kon
)