#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <kon/shm.hpp>

struct shm_test_data {
    std::uint64_t tag;
    double percent;
};

TEST_CASE("shm", "[shm]") {
    using namespace std::literals;
    std::string shm_file{"shm_test_file"sv};
    auto &&shm_file_path = std::filesystem::path{"/dev/shm"} / shm_file;
    std::error_code ec;
    bool is_exist = std::filesystem::exists(shm_file_path, ec);
    REQUIRE_FALSE(ec);

    if (is_exist) {
        bool success = std::filesystem::remove(shm_file_path, ec);
        REQUIRE_FALSE(ec);
        REQUIRE(success);
    }

    {
        int err;
        kon::shm shm(err, shm_file, 1000);
        REQUIRE(err == 0);
        REQUIRE(shm.is_first());

        auto data = new (shm.data()) shm_test_data;
        data->tag = 0x1234567887654321;
        data->percent = 37.125;
    }

    {
        int err;
        kon::shm shm(err, shm_file, 1000);
        REQUIRE(err == 0);
        REQUIRE_FALSE(shm.is_first());

        auto data = static_cast<shm_test_data *>(shm.data());
        REQUIRE(data->tag == 0x1234567887654321);
        REQUIRE(data->percent == 37.125);
    }

    {
        int err;
        kon::shm shm0;
        kon::shm shm1(err, shm_file, 1000);
        REQUIRE(shm0.data() == nullptr);
        REQUIRE(shm1.data() != nullptr);

        shm0 = std::move(shm1);
        REQUIRE(shm0.data() != nullptr);
        REQUIRE(shm1.data() == nullptr);

        REQUIRE_FALSE(shm0.is_first());
    }
    std::filesystem::remove(shm_file_path, ec);
}