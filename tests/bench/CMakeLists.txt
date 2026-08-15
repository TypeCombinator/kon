add_executable(kon_bench
    conv.cpp
)
target_link_libraries(kon_bench PRIVATE
    kon
    benchmark::benchmark
    benchmark::benchmark_main
)