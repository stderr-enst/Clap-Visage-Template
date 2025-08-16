#include "doctest/doctest.h"

#include "parameter.h"

Parameter<double> createTestParameter() {
    return {
        .id = 0,
        .name = "Testparameter",
        .min_value = 0.0,
        .max_value = 1.0,
        .default_value = 0.8,
        .value = 0.1
    };
}

class MockView : public IParameterView<double> {
public:
    void setView(const double& v) override {
        displayvalue = v;
    }

    [[nodiscard]] const double getDisplayValue() const override {
        return displayvalue;
    }

private:
    double displayvalue{0.0};
};

TEST_CASE("Parameter named initialization") {
    auto p = createTestParameter();
    CHECK(p.id == 0);
    CHECK(p.default_value == doctest::Approx(0.8));
    CHECK(p.value == doctest::Approx(0.1));
}

TEST_CASE("ParameterModel initialized parameter to default") {
    auto p = createTestParameter();
    REQUIRE(p.value != doctest::Approx(p.default_value));

    ParameterModel<double> pm = ParameterModel<double>(p);
    auto pp = pm.getParameter();
    CHECK(pp->value == doctest::Approx(pp->default_value));
}

TEST_CASE("ParameterModel setModel") {
    auto p = createTestParameter();
    ParameterModel<double> pm = ParameterModel<double>(p);
    auto pp = pm.getParameter();

    SUBCASE("setModel clamps to min"){
        REQUIRE(pp->min_value == doctest::Approx(0.0));
        pm.setModel(-0.3);
        CHECK(pp->min_value != doctest::Approx(-0.3));
    }

    SUBCASE("setModel clamps to max"){
        REQUIRE(pp->max_value == doctest::Approx(1.0));
        pm.setModel(1.2);
        CHECK(pp->max_value != doctest::Approx(1.2));
    }
}

TEST_CASE("IParameterView registration") {
    auto p = createTestParameter();
    ParameterModel<double> pm = ParameterModel<double>(p);
    auto pp = pm.getParameter();

    auto view = MockView();
    auto anotherview = MockView();

    SUBCASE("register view updates displayvalue") {
        REQUIRE(view.getDisplayValue() == doctest::Approx(0.0));
        pm.registerView(&view);
        CHECK(view.getDisplayValue() == doctest::Approx(pp->value));
    }

    SUBCASE("updateView updates all registered views") {
        REQUIRE(view.getDisplayValue() == doctest::Approx(0.0));
        REQUIRE(anotherview.getDisplayValue() == doctest::Approx(0.0));
        pm.registerView(&view);
        pm.registerView(&anotherview);
        CHECK(view.getDisplayValue() == doctest::Approx(pp->value));
        CHECK(anotherview.getDisplayValue() == doctest::Approx(pp->value));
    }

    SUBCASE("setModel updates views displayvalue"){
        REQUIRE(view.getDisplayValue() == doctest::Approx(0.0));
        REQUIRE(view.getDisplayValue() != doctest::Approx(pp->value));
        pm.registerView(&view);
        pm.setModel(0.3);
        CHECK(pp->value == doctest::Approx(0.3));
        CHECK(view.getDisplayValue() == doctest::Approx(pp->value));
    }
}
