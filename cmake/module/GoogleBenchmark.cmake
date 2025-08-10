set(BENCHMARK_ENABLE_TESTING OFF CACHE INTERNAL "")
FetchContent_Declare(
  GoogleBenchmark
  URL https://github.com/google/benchmark/archive/refs/tags/v1.9.2.tar.gz
  URL_HASH MD5=adcead8dd5416fc6c6245f68ff75a761
  DOWNLOAD_EXTRACT_TIMESTAMP ON
)
FetchContent_MakeAvailable(GoogleBenchmark)