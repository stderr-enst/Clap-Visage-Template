#include "doctest/doctest.h"

#include "Parameter.h"
#include "ParameterList.h"

#include <string>

TEST_CASE("ParameterList basics") {
    Parameter<double> p {
            .id = 0,
            .name = "Testparameter",
            .min_value = 0.0,
            .max_value = 1.0,
            .default_value = 0.8,
            .value = 0.1
    };

    ParameterList<double> pl{};
    auto id = pl.createParameter(
            p.name,
            p.min_value,
            p.max_value,
            p.default_value
            );
    REQUIRE(id == 0);

    SUBCASE("Correct ID for more parameters") {
        auto id2 = pl.createParameter(
                std::string("Testparameter2"),
                0.0,
                1.0,
                1.0
                );
        CHECK(id2 == 1);

        auto id3 = pl.createParameter(
                std::string("Testparameter3"),
                0.0,
                1.0,
                1.0
                );
        CHECK(id3 == 2);

        SUBCASE("Name to ID") {
            auto newid = pl.getParameterId("Testparameter");
            CHECK(id == newid);
            auto newid2 = pl.getParameterId("Testparameter2");
            CHECK(id2 == newid2);

            auto newid3 = pl.getParameterId("Doesn't exist");
            CHECK(!newid3.has_value());
        }
    }

    SUBCASE("Value initializes to default"){
        REQUIRE(p.value != doctest::Approx(p.default_value));

        double value = pl.getParameterValue(id);
        double default_value = pl.getParameterDefault(id);

        CHECK(value == doctest::Approx(default_value));
    }

    SUBCASE("getters"){
        std::string name = pl.getParameterName(id);
        double value = pl.getParameterValue(id);
        double min_value = pl.getParameterMin(id);
        double max_value = pl.getParameterMax(id);
        double default_value = pl.getParameterDefault(id);

        CHECK(name == p.name);
        CHECK(value == doctest::Approx(p.default_value)); // init to default
        CHECK(min_value == doctest::Approx(p.min_value));
        CHECK(max_value == doctest::Approx(p.max_value));
        CHECK(default_value == doctest::Approx(p.default_value));
    }

    SUBCASE("setParameterValue clamps to min"){
        double min_value = pl.getParameterMin(id);
        REQUIRE(min_value == doctest::Approx(0.0));

        pl.setParameterValue(id, -0.3);
        double value = pl.getParameterValue(id);
        CHECK(value > doctest::Approx(-0.3));
        CHECK(value == doctest::Approx(min_value));
    }

    SUBCASE("setParameterValue clamps to max"){
        double max_value = pl.getParameterMax(id);
        REQUIRE(max_value == doctest::Approx(1.0));

        pl.setParameterValue(id, 1.2);
        double value = pl.getParameterValue(id);
        CHECK(value < doctest::Approx(1.2));
        CHECK(value == doctest::Approx(max_value));
    }

    SUBCASE("Other setters"){
        REQUIRE(pl.getParameterName(id) == "Testparameter");
        pl.setParameterName(id, "Testparameter2");
        CHECK(pl.getParameterName(id) == "Testparameter2");

        REQUIRE(pl.getParameterMin(id) == doctest::Approx(0.0));
        pl.setParameterMin(id, 0.1);
        CHECK(pl.getParameterMin(id) == doctest::Approx(0.1));

        REQUIRE(pl.getParameterMax(id) == doctest::Approx(1.0));
        pl.setParameterMax(id, 1.2);
        CHECK(pl.getParameterMax(id) == doctest::Approx(1.2));

        REQUIRE(pl.getParameterDefault(id) == doctest::Approx(0.8));
        pl.setParameterDefault(id, 0.3);
        CHECK(pl.getParameterDefault(id) == doctest::Approx(0.3));

        REQUIRE(pl.getParameterValue(id) == doctest::Approx(0.8));
        pl.setParameterToDefault(id);
        CHECK(pl.getParameterValue(id) == doctest::Approx(pl.getParameterDefault(id)));
    }
}
