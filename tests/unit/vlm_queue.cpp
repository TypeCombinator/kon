#include <catch2/catch_test_macros.hpp>
#include <kon/vlm_queue.hpp>

namespace vlmq_test {
struct message0 {
    uint32_t sn;
    uint32_t sn1;
};

struct message1 {
    uint8_t sn;
};
}; // namespace vlmq_test

TEST_CASE("vlm_queue", "[vlm_queue]") {
    SECTION("basic") {
        kon::vlm_queue q(30);
        kon::vlm_queue::message msg;

        {
            REQUIRE(q.push_begin(msg, sizeof(vlmq_test::message0)));
            msg.head->type = 0x70;
            msg.head->length = sizeof(vlmq_test::message0);
            auto& msg_data = *new (msg.data) vlmq_test::message0;
            msg_data.sn = 100;
            msg_data.sn1 = 1000;
            q.push_end(msg);

            REQUIRE_FALSE(q.push_begin(msg, sizeof(vlmq_test::message1)));
        }

        {
            REQUIRE(q.pop_begin(msg));
            REQUIRE(msg.head->type == 0x70);
            REQUIRE(msg.head->length == sizeof(vlmq_test::message0));
            auto& msg_data = *new (msg.data) vlmq_test::message0;
            REQUIRE(msg_data.sn == 100);
            REQUIRE(msg_data.sn1 == 1000);
            q.pop_end(msg);

            REQUIRE_FALSE(q.pop_begin(msg));
        }

        REQUIRE_FALSE(q.push_begin(msg, sizeof(vlmq_test::message0)));
    }

    SECTION("basic") {
        kon::vlm_queue q(30);
        kon::vlm_queue::message msg;

        {
            REQUIRE(q.push_begin(msg, 0));
            msg.head->type = 0x11;
            msg.head->length = 0;
            q.push_end(msg);
        }

        {
            REQUIRE(q.pop_begin(msg));
            REQUIRE(msg.head->type == 0x11);
            REQUIRE(msg.head->length == 0);
            q.pop_end(msg);

            REQUIRE_FALSE(q.pop_begin(msg));
        }
    }

    SECTION("advanced") {
        kon::vlm_queue q(32);
        kon::vlm_queue::message msg;

        {
            REQUIRE(q.push_begin(msg, sizeof(vlmq_test::message0)));
            msg.head->type = 0x70;
            msg.head->length = sizeof(vlmq_test::message0);
            auto& msg_data = *new (msg.data) vlmq_test::message0;
            msg_data.sn = 100;
            msg_data.sn1 = 1000;
            q.push_end(msg);

            REQUIRE(q.write_index() == 16);
            REQUIRE(q.read_index() == 0);

            REQUIRE(q.push_begin(msg, sizeof(vlmq_test::message1)));
            msg.head->type = 0x71;
            msg.head->length = sizeof(vlmq_test::message1);
            auto& msg_data1 = *new (msg.data) vlmq_test::message1;
            msg_data1.sn = 123;
            q.push_end(msg);

            REQUIRE(q.write_index() == 32);
            REQUIRE(q.read_index() == 0);
        }

        {
            REQUIRE(q.pop_begin(msg));
            REQUIRE(msg.head->type == 0x70);
            REQUIRE(msg.head->length == sizeof(vlmq_test::message0));
            auto& msg_data = *new (msg.data) vlmq_test::message0;
            REQUIRE(msg_data.sn == 100);
            REQUIRE(msg_data.sn1 == 1000);
            q.pop_end(msg);
            REQUIRE(q.write_index() == 32);
            REQUIRE(q.read_index() == 16);

            REQUIRE(q.pop_begin(msg));
            REQUIRE(msg.head->type == 0x71);
            REQUIRE(msg.head->length == sizeof(vlmq_test::message1));
            auto& msg_data1 = *new (msg.data) vlmq_test::message1;
            REQUIRE(msg_data1.sn == 123);
            q.pop_end(msg);
            REQUIRE(q.write_index() == 32);
            REQUIRE(q.read_index() == 32);
        }

        {
            REQUIRE(q.push_begin(msg, sizeof(vlmq_test::message0)));
            msg.head->type = 0x70;
            msg.head->length = sizeof(vlmq_test::message0);
            auto& msg_data = *new (msg.data) vlmq_test::message0;
            msg_data.sn = 200;
            msg_data.sn1 = 2000;
            q.push_end(msg);

            REQUIRE(q.write_index() == 16);
            REQUIRE(q.read_index() == 32);
        }

        {
            REQUIRE(q.pop_begin(msg));
            REQUIRE(msg.head->type == 0x70);
            REQUIRE(msg.head->length == sizeof(vlmq_test::message0));
            auto& msg_data = *new (msg.data) vlmq_test::message0;
            REQUIRE(msg_data.sn == 200);
            REQUIRE(msg_data.sn1 == 2000);
            q.pop_end(msg);
            REQUIRE(q.write_index() == 16);
            REQUIRE(q.read_index() == 16);
        }
    }
}