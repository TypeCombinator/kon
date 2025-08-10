FetchContent_Declare(
    fmt
    URL https://github.com/fmtlib/fmt/archive/refs/tags/11.2.0.tar.gz
    URL_HASH MD5=2f3701cada629ca455c3388d1089f5bd
    DOWNLOAD_EXTRACT_TIMESTAMP ON
    UPDATE_DISCONNECTED true
)
FetchContent_MakeAvailable(fmt)