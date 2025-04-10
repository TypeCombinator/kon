add_executable(kon_tests
    base16.cpp
    dbuf.cpp
    file_helper.cpp
    maybe.cpp
)

target_link_libraries(kon_tests PRIVATE
    Catch2::Catch2WithMain
    kon
)