#include <kon/log/log_sink_circular_buffer.hpp>
#include <kon/log/log.hpp>
#include <catch2/catch_test_macros.hpp>
#include <vector>

TEST_CASE("basic", "[log_sink_cirular_buffer]") {
    constexpr std::size_t sink_buffer_size = 8;
    constexpr std::size_t input_buffer_size = 128;
    kon::log_sink_circular_buffer sink{};
    REQUIRE(sink.initialize(sink_buffer_size) == 0);
    std::vector<char> input_buffer;
    input_buffer.reserve(input_buffer_size);
    for (unsigned i{}; i < input_buffer_size; i++) {
        input_buffer.push_back('0' + (i % 10));
    }

    for (unsigned offset{}; offset < sink_buffer_size; offset++) {
        for (std::size_t i{}; i < (input_buffer_size - sink_buffer_size); i++) {
            sink.reset();
            sink.write_all(&sink, std::string_view{input_buffer.data(), offset});

            sink.write_all(&sink, std::string_view{input_buffer.data() + offset, i});
            REQUIRE(sink.validate() == 0);

            auto l = offset + i;
            auto lq = (l / sink_buffer_size);
            auto lr = (l % sink_buffer_size);
            REQUIRE(sink.m_offset == lr);
            REQUIRE(sink.m_round == lq);
            REQUIRE(
                std::string_view{reinterpret_cast<char *>(sink.m_buffer), lr}
                == std::string_view{input_buffer.data() + (lq * sink_buffer_size), lr});
            if (lq > 0) {
                REQUIRE(
                    std::string_view{
                        reinterpret_cast<char *>(sink.m_buffer) + lr, sink_buffer_size - lr}
                    == std::string_view{
                        input_buffer.data() + ((lq - 1) * sink_buffer_size) + lr,
                        sink_buffer_size - lr});
            }
        }
    }
}

TEST_CASE("get_tail", "[log_sink_cirular_buffer]") {
    constexpr std::size_t sink_buffer_size = 8;
    kon::log_sink_circular_buffer sink{};
    REQUIRE(sink.initialize(sink_buffer_size) == 0);
    SECTION("round0-offset0") {
        kon::log_sink_circular_buffer::tail_space space;
        sink.get_tail(5, space);
        REQUIRE(space.m_first_part == sink.m_buffer);
        REQUIRE(space.m_first_part_size == 0);
        REQUIRE(space.m_second_part == nullptr);
        REQUIRE(space.m_total_size == 0);
    }
    SECTION("round0-offsetX") {
        kon::log_sink_circular_buffer::tail_space space;
        sink.write_all(&sink, std::string_view{"123"});
        sink.get_tail(5, space);
        REQUIRE(space.m_first_part == sink.m_buffer);
        REQUIRE(space.m_first_part_size == 3);
        REQUIRE(space.m_second_part == nullptr);
        REQUIRE(space.m_total_size == 3);

        sink.get_tail(2, space);
        REQUIRE(space.m_first_part == sink.m_buffer + 1);
        REQUIRE(space.m_first_part_size == 2);
        REQUIRE(space.m_second_part == nullptr);
        REQUIRE(space.m_total_size == 2);
    }
    SECTION("round1-offset0") {
        kon::log_sink_circular_buffer::tail_space space;
        sink.write_all(&sink, std::string_view{"12345678"});
        sink.get_tail(5, space);
        REQUIRE(space.m_first_part == sink.m_buffer + 3);
        REQUIRE(space.m_first_part_size == 5);
        REQUIRE(space.m_second_part == sink.m_buffer);
        REQUIRE(space.m_total_size == 5);

        sink.get_tail(9, space);
        REQUIRE(space.m_first_part == sink.m_buffer);
        REQUIRE(space.m_first_part_size == 8);
        REQUIRE(space.m_second_part == sink.m_buffer);
        REQUIRE(space.m_total_size == 8);
    }
    SECTION("round1-offset5") {
        kon::log_sink_circular_buffer::tail_space space;
        sink.write_all(&sink, std::string_view{"1234567890123"});
        sink.get_tail(2, space);
        REQUIRE(space.m_first_part == sink.m_buffer + 3);
        REQUIRE(space.m_first_part_size == 2);
        REQUIRE(space.m_second_part == nullptr);
        REQUIRE(space.m_total_size == 2);

        sink.get_tail(5, space);
        REQUIRE(space.m_first_part == sink.m_buffer);
        REQUIRE(space.m_first_part_size == 5);
        REQUIRE(space.m_second_part == nullptr);
        REQUIRE(space.m_total_size == 5);

        sink.get_tail(7, space);
        REQUIRE(space.m_first_part == sink.m_buffer + 6);
        REQUIRE(space.m_first_part_size == 2);
        REQUIRE(space.m_second_part == sink.m_buffer);
        REQUIRE(space.m_total_size == 7);

        sink.get_tail(9, space);
        REQUIRE(space.m_first_part == sink.m_buffer + 5);
        REQUIRE(space.m_first_part_size == 3);
        REQUIRE(space.m_second_part == sink.m_buffer);
        REQUIRE(space.m_total_size == 8);
    }
}

TEST_CASE("read_slice of tail_space", "[log_sink_cirular_buffer]") {
    constexpr std::size_t sink_buffer_size = 8;
    constexpr std::size_t input_buffer_max_size = sink_buffer_size * 2;
    kon::log_sink_circular_buffer sink{};
    REQUIRE(sink.initialize(sink_buffer_size) == 0);
    std::vector<char> input_buffer;
    input_buffer.reserve(input_buffer_max_size);
    for (unsigned i{}; i < input_buffer_max_size; i++) {
        input_buffer.push_back('0' + (i % 10));
    }

    auto out_buffer = std::make_unique_for_overwrite<std::uint8_t[]>(sink_buffer_size);
    kon::log_sink_circular_buffer::tail_space space;

    for (unsigned i{}; i < input_buffer_max_size; i++) {
        sink.reset();
        sink.write_all(&sink, std::string_view{input_buffer.data(), i});

        for (unsigned tail_size{}; tail_size <= sink_buffer_size; tail_size++) {
            sink.get_tail(tail_size, space);
            std::size_t total_size = space.m_total_size;
            REQUIRE(total_size <= i);
            REQUIRE(total_size <= sink_buffer_size);
            for (unsigned slice_size{1}; slice_size < sink_buffer_size; slice_size++) {
                std::size_t offset{};
                bool is_last{};
                do {
                    std::size_t ret_slice_size =
                        space.read_slice(offset, out_buffer.get() + offset, slice_size, is_last);
                    offset += ret_slice_size;
                } while (!is_last);
                REQUIRE(total_size == offset);

                std::string_view input_buffer_tail{input_buffer.data() + i - total_size, total_size};
                std::string_view output_buffer_result{
                    reinterpret_cast<char *>(out_buffer.get()), offset};
                REQUIRE(input_buffer_tail == output_buffer_result);
            }
        }
    }
}

#define TEST_LOG_TRC(_log_, _fmt_, ...)                                                            \
    KON_LOG_PRINT_FLEVEL(_log_, "TRC", kon::log_level::trace, _fmt_, ##__VA_ARGS__)

#define TEST_LOG_DBG(_log_, _fmt_, ...)                                                            \
    KON_LOG_PRINT_FLEVEL(_log_, "DBG", kon::log_level::debug, _fmt_, ##__VA_ARGS__)

#define TEST_LOG_INF(_log_, _fmt_, ...)                                                            \
    KON_LOG_PRINT_FLEVEL(_log_, "INF", kon::log_level::information, _fmt_, ##__VA_ARGS__)

#define TEST_LOG_WAR(_log_, _fmt_, ...)                                                            \
    KON_LOG_PRINT_FLEVEL(_log_, "WAR", kon::log_level::warning, _fmt_, ##__VA_ARGS__)

#define TEST_LOG_ERR(_log_, _fmt_, ...)                                                            \
    KON_LOG_PRINT_FLEVEL(_log_, "ERR", kon::log_level::error, _fmt_, ##__VA_ARGS__)

#define TEST_LOG_CRT(_log_, _fmt_, ...)                                                            \
    KON_LOG_PRINT_FLEVEL(_log_, "CRT", kon::log_level::critical, _fmt_, ##__VA_ARGS__)

TEST_CASE("log", "[log_sink_cirular_buffer]") {
    kon::log_sink_circular_buffer sink;
    sink.initialize(1024);
    kon::logger log;
    log.set_sink(sink.sink_if, &sink);
    log.set_level(kon::log_level::information);
    TEST_LOG_ERR(log, "hello");
    TEST_LOG_DBG(log, "world");

    kon::log_sink_circular_buffer::tail_space space;
    sink.get_tail(512, space);
    REQUIRE(space.m_total_size != 0);
    REQUIRE(space.m_total_size == space.m_first_part_size);

    std::string_view content{
        reinterpret_cast<const char *>(space.m_first_part), space.m_first_part_size};
    REQUIRE(content.find("hello") != std::string_view::npos);
    REQUIRE(content.find("world") == std::string_view::npos);

    sink.reset();
    sink.get_tail(512, space);
    REQUIRE(space.m_total_size == 0);
}