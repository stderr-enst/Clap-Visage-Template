#include "doctest/doctest.h"

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

        auto value = pl.getParameterValue(id);
        REQUIRE( value.has_value() );
        auto default_value = pl.getParameterDefault(id);
        REQUIRE( default_value.has_value() );

        CHECK(value.value_or(0.0) == doctest::Approx(default_value.value_or(0.8)));
    }

    SUBCASE("getters by ID"){
        auto name = pl.getParameterName(id);
        REQUIRE( name.has_value() );
        auto value = pl.getParameterValue(id);
        REQUIRE( value.has_value() );
        auto min_value = pl.getParameterMin(id);
        REQUIRE( min_value.has_value() );
        auto max_value = pl.getParameterMax(id);
        REQUIRE( max_value.has_value() );
        auto default_value = pl.getParameterDefault(id);
        REQUIRE( default_value.has_value() );

        CHECK(name.value_or("") == p.name);
        CHECK(value.value_or(0.0) == doctest::Approx(p.default_value)); // init to default
        CHECK(min_value.value_or(1.0) == doctest::Approx(p.min_value));
        CHECK(max_value.value_or(0.0) == doctest::Approx(p.max_value));
        CHECK(default_value.value_or(0.0) == doctest::Approx(p.default_value));
    }

    SUBCASE("getters by name"){
        const std::string name{"Testparameter"};
        auto checkid = pl.getParameterId(name);
        REQUIRE( checkid.has_value() );
        auto value = pl.getParameterValue(name);
        REQUIRE( value.has_value() );
        auto min_value = pl.getParameterMin(name);
        REQUIRE( min_value.has_value() );
        auto max_value = pl.getParameterMax(name);
        REQUIRE( max_value.has_value() );
        auto default_value = pl.getParameterDefault(name);
        REQUIRE( default_value.has_value() );

        CHECK(checkid.value_or(3) == p.id);
        CHECK(value.value_or(0.0) == doctest::Approx(p.default_value)); // init to default
        CHECK(min_value.value_or(1.0) == doctest::Approx(p.min_value));
        CHECK(max_value.value_or(0.0) == doctest::Approx(p.max_value));
        CHECK(default_value.value_or(0.0) == doctest::Approx(p.default_value));
    }

    SUBCASE("setParameterValue clamps to min"){
        auto min_value = pl.getParameterMin(id);
        REQUIRE(min_value.value_or(1.0) == doctest::Approx(0.0));

        pl.setParameterValue(id, -0.3);
        auto value = pl.getParameterValue(id);
        CHECK(value.value_or(0.0) > doctest::Approx(-0.3));
        CHECK(value.value_or(0.0) == doctest::Approx(min_value.value_or(1.0)));
    }

    SUBCASE("setParameterValue clamps to max"){
        auto max_value = pl.getParameterMax(id);
        REQUIRE(max_value.value_or(0.0) == doctest::Approx(1.0));

        pl.setParameterValue(id, 1.2);
        auto value = pl.getParameterValue(id);
        CHECK(value.value_or(0.0) < doctest::Approx(1.2));
        CHECK(value.value_or(1.0) == doctest::Approx(max_value.value_or(0.0)));
    }

    SUBCASE("Remaining setters by ID"){
        REQUIRE(pl.getParameterName(id).value_or("") == "Testparameter");
        pl.setParameterName(id, "Testparameter2");
        CHECK(pl.getParameterName(id).value_or("") == "Testparameter2");

        REQUIRE(pl.getParameterMin(id).value_or(1.0) == doctest::Approx(0.0));
        pl.setParameterMin(id, 0.1);
        CHECK(pl.getParameterMin(id).value_or(1.0) == doctest::Approx(0.1));

        REQUIRE(pl.getParameterMax(id).value_or(0.0) == doctest::Approx(1.0));
        pl.setParameterMax(id, 1.2);
        CHECK(pl.getParameterMax(id).value_or(0.0) == doctest::Approx(1.2));

        REQUIRE(pl.getParameterDefault(id).value_or(0.0) == doctest::Approx(0.8));
        pl.setParameterDefault(id, 0.3);
        CHECK(pl.getParameterDefault(id).value_or(0.0) == doctest::Approx(0.3));

        REQUIRE(pl.getParameterValue(id).value_or(0.0) == doctest::Approx(0.8));
        pl.setParameterToDefault(id);
        CHECK(pl.getParameterValue(id).value_or(0.0) == doctest::Approx(pl.getParameterDefault(id).value_or(0.8)));

        auto paramObject = pl.getParameter(id);
        if (paramObject.has_value()) {
            CHECK(paramObject.value()->id == 0);
            CHECK(paramObject.value()->name == "Testparameter2");
            CHECK(paramObject.value()->min_value == doctest::Approx(0.1));
            CHECK(paramObject.value()->max_value == doctest::Approx(1.2));
            CHECK(paramObject.value()->default_value == doctest::Approx(0.3));
            CHECK(paramObject.value()->value == doctest::Approx(0.3));
        } else {
            REQUIRE(false);
        }
    }

    SUBCASE("Remaining setters by name"){
        const std::string name{"Testparameter"};

        REQUIRE(pl.getParameterValue(name).value_or(0.0) != doctest::Approx(0.3));
        pl.setParameterValue(name, 0.3);
        CHECK(pl.getParameterValue(name).value_or(0.0) == doctest::Approx(0.3));

        REQUIRE(pl.getParameterMin(name).value_or(1.0) == doctest::Approx(0.0));
        pl.setParameterMin(name, 0.1);
        CHECK(pl.getParameterMin(name).value_or(0.0) == doctest::Approx(0.1));

        REQUIRE(pl.getParameterMax(name).value_or(0.0) == doctest::Approx(1.0));
        pl.setParameterMax(name, 1.2);
        CHECK(pl.getParameterMax(name).value_or(0.0) == doctest::Approx(1.2));

        REQUIRE(pl.getParameterDefault(name).value_or(0.0) == doctest::Approx(0.8));
        pl.setParameterDefault(name, 0.3);
        CHECK(pl.getParameterDefault(name).value_or(0.0) == doctest::Approx(0.3));

        auto paramObject = pl.getParameter(name);
        if (paramObject.has_value()) {
            CHECK(paramObject.value()->id == 0);
            CHECK(paramObject.value()->name == "Testparameter");
            CHECK(paramObject.value()->min_value == doctest::Approx(0.1));
            CHECK(paramObject.value()->max_value == doctest::Approx(1.2));
            CHECK(paramObject.value()->default_value == doctest::Approx(0.3));
            CHECK(paramObject.value()->value == doctest::Approx(0.3));
        } else {
            REQUIRE(false);
        }
    }
}
