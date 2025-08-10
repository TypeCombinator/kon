#include <catch2/catch_test_macros.hpp>
#include <kon/vlm_ring.hpp>
#include <thread>
#include <vector>

namespace vlmq_test {
struct message0 {
    uint32_t sn;
    uint32_t sn1;
};

struct message1 {
    uint8_t sn;
};
}; // namespace vlmq_test

TEST_CASE("vlm_ring", "[vlm_ring]") {
    SECTION("basic") {
        kon::vlm_ring q(30);
        kon::vlm_ring::zc_scope zcs;

        {
            REQUIRE(q.push_begin(zcs, sizeof(vlmq_test::message0)));
            zcs.head->type = 0x70;
            zcs.head->length = sizeof(vlmq_test::message0);
            auto& msg_data = *new (zcs.data) vlmq_test::message0;
            msg_data.sn = 100;
            msg_data.sn1 = 1000;
            q.push_end(zcs);

            REQUIRE_FALSE(q.push_begin(zcs, sizeof(vlmq_test::message1)));
        }

        {
            REQUIRE(q.pop_begin(zcs));
            REQUIRE(zcs.head->type == 0x70);
            REQUIRE(zcs.head->length == sizeof(vlmq_test::message0));
            auto& msg_data = *new (zcs.data) vlmq_test::message0;
            REQUIRE(msg_data.sn == 100);
            REQUIRE(msg_data.sn1 == 1000);
            q.pop_end(zcs);

            REQUIRE_FALSE(q.pop_begin(zcs));
        }

        REQUIRE_FALSE(q.push_begin(zcs, sizeof(vlmq_test::message0)));
    }

    SECTION("basic") {
        kon::vlm_ring q(30);
        kon::vlm_ring::zc_scope zcs;

        {
            REQUIRE(q.push_begin(zcs, 0));
            zcs.head->type = 0x11;
            zcs.head->length = 0;
            q.push_end(zcs);
        }

        {
            REQUIRE(q.pop_begin(zcs));
            REQUIRE(zcs.head->type == 0x11);
            REQUIRE(zcs.head->length == 0);
            q.pop_end(zcs);

            REQUIRE_FALSE(q.pop_begin(zcs));
        }
    }

    SECTION("advanced") {
        kon::vlm_ring q(32);
        kon::vlm_ring::zc_scope zcs;

        {
            REQUIRE(q.push_begin(zcs, sizeof(vlmq_test::message0)));
            zcs.head->type = 0x70;
            zcs.head->length = sizeof(vlmq_test::message0);
            auto& msg_data = *new (zcs.data) vlmq_test::message0;
            msg_data.sn = 100;
            msg_data.sn1 = 1000;
            q.push_end(zcs);

            REQUIRE(q.write_index() == 16);
            REQUIRE(q.read_index() == 0);

            REQUIRE(q.push_begin(zcs, sizeof(vlmq_test::message1)));
            zcs.head->type = 0x71;
            zcs.head->length = sizeof(vlmq_test::message1);
            auto& msg_data1 = *new (zcs.data) vlmq_test::message1;
            msg_data1.sn = 123;
            q.push_end(zcs);

            REQUIRE(q.write_index() == 32);
            REQUIRE(q.read_index() == 0);
        }

        {
            REQUIRE(q.pop_begin(zcs));
            REQUIRE(zcs.head->type == 0x70);
            REQUIRE(zcs.head->length == sizeof(vlmq_test::message0));
            auto& msg_data = *new (zcs.data) vlmq_test::message0;
            REQUIRE(msg_data.sn == 100);
            REQUIRE(msg_data.sn1 == 1000);
            q.pop_end(zcs);
            REQUIRE(q.write_index() == 32);
            REQUIRE(q.read_index() == 16);

            REQUIRE(q.pop_begin(zcs));
            REQUIRE(zcs.head->type == 0x71);
            REQUIRE(zcs.head->length == sizeof(vlmq_test::message1));
            auto& msg_data1 = *new (zcs.data) vlmq_test::message1;
            REQUIRE(msg_data1.sn == 123);
            q.pop_end(zcs);
            REQUIRE(q.write_index() == 32);
            REQUIRE(q.read_index() == 32);
        }

        {
            REQUIRE(q.push_begin(zcs, sizeof(vlmq_test::message0)));
            zcs.head->type = 0x70;
            zcs.head->length = sizeof(vlmq_test::message0);
            auto& msg_data = *new (zcs.data) vlmq_test::message0;
            msg_data.sn = 200;
            msg_data.sn1 = 2000;
            q.push_end(zcs);

            REQUIRE(q.write_index() == 16);
            REQUIRE(q.read_index() == 32);
        }

        {
            REQUIRE(q.pop_begin(zcs));
            REQUIRE(zcs.head->type == 0x70);
            REQUIRE(zcs.head->length == sizeof(vlmq_test::message0));
            auto& msg_data = *new (zcs.data) vlmq_test::message0;
            REQUIRE(msg_data.sn == 200);
            REQUIRE(msg_data.sn1 == 2000);
            q.pop_end(zcs);
            REQUIRE(q.write_index() == 16);
            REQUIRE(q.read_index() == 16);
        }
    }
}

TEST_CASE("vlm_ring_multi_thread", "[vlm_ring]") {
    constexpr uint32_t max_item_num = 300000;
    kon::vlm_ring q(max_item_num / 10);

    auto producer = [&q]() -> void {
        for (uint32_t i = 0; i < max_item_num; i++) {
            while (!q.push(i)) {
                std::this_thread::yield();
            }
        }
    };

    std::vector<uint32_t> consumer_recorder;
    consumer_recorder.reserve(max_item_num);

    auto consumer = [&q, &consumer_recorder]() -> void {
        kon::vlm_ring::zc_scope zcs;
        for (uint32_t i = 0; i < max_item_num; i++) {
            while (!q.pop_begin(zcs)) {
                std::this_thread::yield();
            }
            uint32_t type = zcs.head->type;
            q.pop_end(zcs);

            consumer_recorder.push_back(type);
        }
    };
    std::thread consumer_thread(consumer);
    std::thread producer_thread(producer);

    producer_thread.join();
    consumer_thread.join();

    REQUIRE(consumer_recorder.size() == max_item_num);
    uint32_t i = 0;
    for (auto item: consumer_recorder) {
        REQUIRE(i == item);
        i++;
    }
}