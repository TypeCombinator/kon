add_executable(kon_tests
    conv.cpp
    dbuf.cpp
    file_helper.cpp
    maybe.cpp
    string_helper.cpp
    vlm_ring.cpp
)

target_link_libraries(kon_tests PRIVATE
    Catch2::Catch2WithMain
    kon
)