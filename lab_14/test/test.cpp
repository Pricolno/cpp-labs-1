#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"

#include "linq.hpp"

#include <sstream>

TEST_SUITE("to_vector") {
    TEST_CASE("empty") {
        std::vector<int> empty;

        auto res = linq::from(empty.begin(), empty.end())
                .to_vector();

        CHECK(res.empty());
    }

    TEST_CASE("non-empty") {
        std::vector<int> some_data = {1, 2, 3};

        auto res = linq::from(some_data.begin(), some_data.end())
                .to_vector();

        CHECK(res == some_data);
    }

    TEST_CASE("initial container does not change") {
        std::vector<int> some_data = {1, 2, 3};
        std::vector<int> initial = some_data;

        linq::from(some_data.begin(), some_data.end())
                .to_vector();

        CHECK(initial == some_data);
    }

    TEST_CASE("input iterator") {
        std::stringstream ss("1 2 3 4 5");
        std::istream_iterator<int> in(ss), eof;

        auto res = linq::from(in, eof)
                .to_vector();

        std::vector<int> expected = {1, 2, 3, 4, 5};
        CHECK(res == expected);
    }
}

TEST_SUITE("copy_to") {
    TEST_CASE("empty") {
        std::vector<int> empty, res;

        linq::from(empty.begin(), empty.end())
                .copy_to(std::back_inserter(res));

        CHECK(res.empty());
    }
}

TEST_SUITE("take") {
    TEST_CASE("take some on empty") {
        std::vector<int> empty, res;

        linq::from(empty.begin(), empty.end())
                .take(5)
                .copy_to(std::back_inserter(res));

        CHECK(res.empty());
    }

    TEST_CASE("take more than there is") {
        std::vector<int> some_data = {1, 2};

        auto res = linq::from(some_data.begin(), some_data.end())
                .take(5)
                .to_vector();

        CHECK(res == some_data);
    }

    TEST_CASE("take exactly") {
        std::vector<int> some_data = {1, 2, 3};

        auto res = linq::from(some_data.begin(), some_data.end())
                .take(2)
                .to_vector();

        std::vector<int> expected = {1, 2};
        CHECK(res == expected);
    }


    TEST_CASE("take exactly") {
        std::vector<int> some_data = {1, 2, 3};

        auto res = linq::from(some_data.begin(), some_data.end())
                .take(2)
                .to_vector();

        std::vector<int> expected = {1, 2};
        CHECK(res == expected);
    }

    TEST_CASE("sstream") {
        std::stringstream ss("1 2 3 4 5");
        std::istream_iterator<int> in(ss), eof;

        std::vector<int> res =
                linq::from(in, eof)
                        .take(4)
                        .to_vector();

        std::vector<int> expected = {1, 2, 3, 4};
        CHECK(expected == res);

        int remaining;
        REQUIRE((ss >> remaining));
        CHECK(remaining == 5);
    }
}

TEST_SUITE("drop") {
    TEST_CASE("drop some on empty") {
        std::vector<int> empty, res;

        linq::from(empty.begin(), empty.end())
                .drop(5)
                .copy_to(std::back_inserter(res));

        CHECK(res.empty());
    }

    TEST_CASE("drop more than there is") {
        std::vector<int> some_data = {1, 2};

        auto res = linq::from(some_data.begin(), some_data.end())
                .drop(5)
                .to_vector();

        CHECK(res.empty());
    }

    TEST_CASE("drop some") {
        std::vector<int> some_data = {1, 2, 3};

        auto res = linq::from(some_data.begin(), some_data.end())
                .drop(1)
                .to_vector();

        std::vector<int> expected = {2, 3};
        CHECK(res == expected);
    }

    TEST_CASE("double drop") {
        std::vector<int> some_data = {1, 2, 3};

        auto res = linq::from(some_data.begin(), some_data.end())
                .drop(1)
                .drop(1)
                .to_vector();

        std::vector<int> expected = {3};
        CHECK(res == expected);
    }

    TEST_CASE("sstream") {
        std::stringstream ss("1 2 3 4 5");
        std::istream_iterator<int> in(ss), eof;

        std::vector<int> res =
                linq::from(in, eof)
                        .drop(1)
                        .take(3)
                        .to_vector();

        std::vector<int> expected = {2, 3, 4};
        CHECK(expected == res);

        int remaining;
        REQUIRE((ss >> remaining));
        CHECK(remaining == 5);
    }
}

TEST_SUITE("until") {
    TEST_CASE("elem is not encountered") {
        std::vector<int> some_data = {1, 2, 3};

        auto res = linq::from(some_data.begin(), some_data.end())
                .until_eq(-1)
                .to_vector();

        CHECK(res == some_data);
    }

    TEST_CASE("until some") {
        std::vector<int> some_data = {1, 2, 3};

        auto res = linq::from(some_data.begin(), some_data.end())
                .until_eq(1)
                .to_vector();

        CHECK(res.empty());
    }

    TEST_CASE("double until") {
        std::vector<int> some_data = {1, 2, 3, 4};

        auto res = linq::from(some_data.begin(), some_data.end())
                .until_eq(4)
                .until_eq(3)
                .to_vector();

        std::vector<int> expected = {1, 2};
        CHECK(res == expected);
    }

    TEST_CASE("sstream") {
        std::stringstream ss("1 2 3 4 5");
        std::istream_iterator<int> in(ss), eof;

        std::vector<int> res =
                linq::from(in, eof)
                        .until_eq(4)
                        .to_vector();

        std::vector<int> expected = {1, 2, 3};
        CHECK(expected == res);

        int remaining;
        REQUIRE((ss >> remaining));
        CHECK(remaining == 5);
    }

    TEST_CASE("filatov's test") {
        auto xs = std::vector<int>{10, 20, 30};
        auto res = linq::from(xs.begin(), xs.end()).until([](int x) {
            CAPTURE(x);
            CHECK((x == 10 || x == 20 || x == 30));
            return false;
        }).to_vector();
        CHECK((std::vector<int>{10, 20, 30}) == res);
    }
}

TEST_SUITE("where") {
    TEST_CASE("leave unchanged") {
        std::vector<int> some_data = {1, 2, 3};

        auto res = linq::from(some_data.begin(), some_data.end())
                .where_neq(4)
                .to_vector();

        CHECK(res == some_data);
    }

    TEST_CASE("filter some") {
        std::vector<int> some_data = {1, 2, 3, 2};

        auto res = linq::from(some_data.begin(), some_data.end())
                .where_neq(2)
                .to_vector();

        std::vector<int> expected = {1, 3};
        CHECK(res == expected);
    }

    TEST_CASE("double where") {
        std::vector<int> some_data = {1, 2, 3, 4};

        auto res = linq::from(some_data.begin(), some_data.end())
                .where_neq(3)
                .where_neq(2)
                .to_vector();

        std::vector<int> expected = {1, 4};
        CHECK(res == expected);
    }

    TEST_CASE("sstream") {
        std::stringstream ss("1 2 3 4 5");
        std::istream_iterator<int> in(ss), eof;

        std::vector<int> res =
                linq::from(in, eof)
                        .where_neq(2)
                        .take(3)
                        .to_vector();

        std::vector<int> expected = {1, 3, 4};
        CHECK(expected == res);

        int remaining;
        REQUIRE((ss >> remaining));
        CHECK(remaining == 5);
    }
}