add_library(kon STATIC
    base16.cpp
    conv.cpp
    file_helper.cpp
)
target_include_directories(kon PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..)