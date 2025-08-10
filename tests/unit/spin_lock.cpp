#include <catch2/catch_test_macros.hpp>
#include <kon/spin_lock.hpp>

TEST_CASE("spin_lock", "[spin_lock]") {
    kon::spin_lock lock;
    int shared_value = 0;

    SECTION("single thread lock/unlock") {
        lock.lock();
        shared_value = 100;
        lock.unlock();
        REQUIRE(shared_value == 100);

        lock.lock();
        shared_value = 200;
        lock.unlock();
        REQUIRE(shared_value == 200);
    }

    SECTION("multiple threads synchronized start") {
        constexpr int thread_count = 4;
        constexpr int iterations = 10000;
        std::vector<std::thread> threads;
        std::atomic<int> ready_count{0};
        std::atomic<bool> go{false};
        int counter = 0;

        for (int i = 0; i < thread_count; ++i) {
            threads.emplace_back([&]() {
                ready_count++;
                while (!go) { /* spin until all threads are ready */
                }

                for (int j = 0; j < iterations; ++j) {
                    lock.lock();
                    counter++;
                    lock.unlock();
                }
            });
        }

        // Wait for all threads to be ready
        while (ready_count < thread_count) {
            std::this_thread::yield();
        }
        go = true; // Start all threads at once

        for (auto& t: threads) {
            t.join();
        }

        REQUIRE(counter == thread_count * iterations);
    }

    SECTION("try lock contention") {
        lock.lock();

        std::thread t([&]() {
            // This should block until main thread unlocks
            lock.lock();
            shared_value = 2;
            lock.unlock();
        });

        REQUIRE(shared_value == 0);
        shared_value = 1;
        lock.unlock();
        t.join();
        REQUIRE(shared_value == 2);
    }
}