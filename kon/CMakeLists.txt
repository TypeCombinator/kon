add_library(kon STATIC
    file_helper.cpp
)
target_include_directories(kon PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..)