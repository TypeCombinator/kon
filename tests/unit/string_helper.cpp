#include <catch2/catch_test_macros.hpp>
#include <kon/string_helper.hpp>

TEST_CASE("string_splitter", "[string_helper]") {
    SECTION("empty0") {
        std::string input{""};
        kon::string_splitter<';'> splitter(input.data(), input.size());
        std::string_view result;
        REQUIRE_FALSE(splitter.next(result));
    }

    SECTION("empty1") {
        std::string input{";"};
        kon::string_splitter<';'> splitter(input.data(), input.size());
        std::string_view result;
        REQUIRE_FALSE(splitter.next(result));
    }

    SECTION("empty2") {
        std::string input{";"};
        kon::string_splitter<';'> splitter(input.data(), input.size());
        std::string_view result;
        REQUIRE_FALSE(splitter.next(result));
    }


    SECTION("one0") {
        std::string input{"abc"};
        kon::string_splitter<';'> splitter(input.data(), input.size());
        std::string_view result;
        REQUIRE(splitter.next(result));
        REQUIRE(result == "abc");

        REQUIRE_FALSE(splitter.next(result));
    }

    SECTION("one1") {
        std::string input{";abc"};
        kon::string_splitter<';'> splitter(input.data(), input.size());
        std::string_view result;
        REQUIRE(splitter.next(result));
        REQUIRE(result == "abc");

        REQUIRE_FALSE(splitter.next(result));
    }

    SECTION("one2") {
        std::string input{";;abc"};
        kon::string_splitter<';'> splitter(input.data(), input.size());
        std::string_view result;
        REQUIRE(splitter.next(result));
        REQUIRE(result == "abc");

        REQUIRE_FALSE(splitter.next(result));
    }

    SECTION("one3") {
        std::string input{";;abc;"};
        kon::string_splitter<';'> splitter(input.data(), input.size());
        std::string_view result;
        REQUIRE(splitter.next(result));
        REQUIRE(result == "abc");

        REQUIRE_FALSE(splitter.next(result));
    }

    SECTION("one4") {
        std::string input{";;abc;;"};
        kon::string_splitter<';'> splitter(input.data(), input.size());
        std::string_view result;
        REQUIRE(splitter.next(result));
        REQUIRE(result == "abc");

        REQUIRE_FALSE(splitter.next(result));
    }


    SECTION("two0") {
        std::string input{";;abc;;defg"};
        kon::string_splitter<';'> splitter(input.data(), input.size());
        std::string_view result;
        REQUIRE(splitter.next(result));
        REQUIRE(result == "abc");

        REQUIRE(splitter.next(result));
        REQUIRE(result == "defg");

        REQUIRE_FALSE(splitter.next(result));
    }

    SECTION("two0") {
        std::string input{";;abc;;defg;;"};
        kon::string_splitter<';'> splitter(input.data(), input.size());
        std::string_view result;
        REQUIRE(splitter.next(result));
        REQUIRE(result == "abc");

        REQUIRE(splitter.next(result));
        REQUIRE(result == "defg");

        REQUIRE_FALSE(splitter.next(result));
    }
}