#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <fstream>
#include <kon/file_helper.hpp>

static int read_all_to_string(const std::string& filePath, std::string& file_content) {
    std::ifstream file(filePath, std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
        return -1;
    }

    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    file_content.resize(fileSize);

    if (!file.read(&file_content[0], fileSize)) {
        return -1;
    }
    return 0;
}

TEST_CASE("read_all", "[file_helper]") {
    std::string file_path{"tests/unit/CMakeLists.txt"};
    std::size_t file_size;
    auto file_data = kon::file_helper::read_all(file_path, file_size);
    REQUIRE(file_data != nullptr);

    std::string file_content{};
    REQUIRE(read_all_to_string(file_path, file_content) == 0);

    CHECK(file_content == std::string_view{reinterpret_cast<char*>(file_data.get()), file_size});
}