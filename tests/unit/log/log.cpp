#include <kon/log/log_sink_circular_buffer.hpp>
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
        std::size_t ret = sink.get_tail(5, space);
        REQUIRE(ret == 0);
        REQUIRE(space.first_part == sink.m_buffer);
        REQUIRE(space.first_part_size == 0);
        REQUIRE(space.second_part == nullptr);
        REQUIRE(space.second_part_size == 0);
    }
    SECTION("round0-offsetX") {
        kon::log_sink_circular_buffer::tail_space space;
        sink.write_all(&sink, std::string_view{"123"});
        std::size_t ret = sink.get_tail(5, space);
        REQUIRE(ret == 3);
        REQUIRE(space.first_part == sink.m_buffer);
        REQUIRE(space.first_part_size == 3);
        REQUIRE(space.second_part == nullptr);
        REQUIRE(space.second_part_size == 0);

        ret = sink.get_tail(2, space);
        REQUIRE(ret == 2);
        REQUIRE(space.first_part == sink.m_buffer + 1);
        REQUIRE(space.first_part_size == 2);
        REQUIRE(space.second_part == nullptr);
        REQUIRE(space.second_part_size == 0);
    }
    SECTION("round1-offset0") {
        kon::log_sink_circular_buffer::tail_space space;
        sink.write_all(&sink, std::string_view{"12345678"});
        std::size_t ret = sink.get_tail(5, space);
        REQUIRE(ret == 5);
        REQUIRE(space.first_part == sink.m_buffer + 3);
        REQUIRE(space.first_part_size == 5);
        REQUIRE(space.second_part == sink.m_buffer);
        REQUIRE(space.second_part_size == 0);

        ret = sink.get_tail(9, space);
        REQUIRE(ret == 8);
        REQUIRE(space.first_part == sink.m_buffer);
        REQUIRE(space.first_part_size == 8);
        REQUIRE(space.second_part == sink.m_buffer);
        REQUIRE(space.second_part_size == 0);
    }
    SECTION("round1-offset5") {
        kon::log_sink_circular_buffer::tail_space space;
        sink.write_all(&sink, std::string_view{"1234567890123"});
        std::size_t ret = sink.get_tail(2, space);
        REQUIRE(ret == 2);
        REQUIRE(space.first_part == sink.m_buffer + 3);
        REQUIRE(space.first_part_size == 2);
        REQUIRE(space.second_part == nullptr);
        REQUIRE(space.second_part_size == 0);

        ret = sink.get_tail(5, space);
        REQUIRE(ret == 5);
        REQUIRE(space.first_part == sink.m_buffer);
        REQUIRE(space.first_part_size == 5);
        REQUIRE(space.second_part == nullptr);
        REQUIRE(space.second_part_size == 0);

        ret = sink.get_tail(7, space);
        REQUIRE(ret == 7);
        REQUIRE(space.first_part == sink.m_buffer + 6);
        REQUIRE(space.first_part_size == 2);
        REQUIRE(space.second_part == sink.m_buffer);
        REQUIRE(space.second_part_size == 5);

        ret = sink.get_tail(9, space);
        REQUIRE(ret == 8);
        REQUIRE(space.first_part == sink.m_buffer + 5);
        REQUIRE(space.first_part_size == 3);
        REQUIRE(space.second_part == sink.m_buffer);
        REQUIRE(space.second_part_size == 5);
    }
}