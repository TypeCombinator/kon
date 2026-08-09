#include <kon/tools/bash.hpp>
#include <catch2/catch_test_macros.hpp>
#include <fmt/base.h>

static const char* xxx_scripts{0 ? nullptr
#include "echo.sh"
};

TEST_CASE("basic", "[tools/bash]") {
    std::string output;
    int error = kon::execute_bash_script(xxx_scripts, "hello world", output);
    REQUIRE(error == 0);
    REQUIRE(output == "hello world");

    output.clear();
    error = kon::execute_bash_script(xxx_scripts, "hello", output);
    REQUIRE(error == 1);
    REQUIRE(output == "invalid argument number: 1");
}