#include <benchmark/benchmark.h>
#include <kon/conv.hpp>
#include <charconv>
#include <random>

template <typename T>
static void random_vector_generator(std::vector<std::string>& v, size_t v_cap, T seed) {
    std::mt19937 gen(seed);
    std::uniform_int_distribution<T> dis{};
    for (; v_cap > 0; v_cap--) {
        v.push_back(std::to_string(dis(gen)));
    }
}

static void bm_std_from_chars(benchmark::State& state) {
    std::vector<std::string> v;
    v.reserve(10000);
    random_vector_generator<std::uint32_t>(v, v.capacity(), 17);
    for (auto _: state) {
        for (const auto& e: v) {
            std::uint32_t value;
            auto [ptr, ec] = std::from_chars(e.data(), e.data() + e.size(), value);
            benchmark::DoNotOptimize(value);
            benchmark::DoNotOptimize(ec);
        }
    }
}

BENCHMARK(bm_std_from_chars);

static void bm_string10_to_int(benchmark::State& state) {
    std::vector<std::string> v;
    v.reserve(10000);
    random_vector_generator<std::uint32_t>(v, v.capacity(), 17);
    for (auto _: state) {
        for (const auto& e: v) {
            std::uint32_t value;
            std::size_t pos = kon::string10_to_uint(e.data(), e.size(), value);
            benchmark::DoNotOptimize(value);
            benchmark::DoNotOptimize(pos);
        }
    }
}

BENCHMARK(bm_string10_to_int);