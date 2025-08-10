cmake_minimum_required(VERSION 3.20)

# include(cmake/toolchain/clang.cmake)

# list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/module")

option(KON_BUILD_TESTS "Build tests" OFF)
option(KON_BUILD_EXAMPLES "Build examples" ON)

project(project_kon LANGUAGES C CXX)

if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
    set(KON_IS_TOP_LEVEL_PROJECT ON)
else()
    set(KON_IS_TOP_LEVEL_PROJECT OFF)
endif()

if(KON_IS_TOP_LEVEL_PROJECT)
    include(FetchContent)

    # Set C++ standard.
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)

    # Add extra c/c++ flags.
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_COMPILER_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_COMPILER_FLAGS}")
endif()

add_subdirectory(kon)

if(KON_BUILD_EXAMPLES)
    # add_subdirectory(examples)
endif()

if(KON_BUILD_TESTING)
    include(cmake/module/Catch2.cmake)
    add_subdirectory(tests/unit)
endif()