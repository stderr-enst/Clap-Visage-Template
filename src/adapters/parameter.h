#ifndef PARAMETER_H
#define PARAMETER_H

#include <algorithm>
#include <concepts>
#include <memory>
#include <string>
#include <vector>

template<typename T>
struct Parameter {
    unsigned int id{};
    std::string name{};
    T min_value{};
    T max_value{};
    T default_value{};
    T value{};
};

template<typename T>
class IParameterView {
public:
    virtual ~IParameterView() = default;

    virtual void setView(const T& v) = 0;
    [[nodiscard]] virtual const T getDisplayValue() const = 0;
};

template<typename T> requires std::totally_ordered<T>
class ParameterModel {
public:
    ParameterModel( Parameter<T>& p ) : parameter{std::move(p)} {
        setToDefault();
    }

    void setToDefault() {
        setModel(parameter.default_value);
    }

    void setModel(const T& newval) {
        auto clampedValue = std::clamp(newval, parameter.min_value, parameter.max_value);
        parameter.value = clampedValue;

        updateViews();
    }

    void registerView(IParameterView<T>* view) {
        if (view != nullptr) {
            views.push_back(view);
        }

        updateViews();
    }

    void updateViews() const {
        for (auto view : views) {
            if (view != nullptr) {
                view->setView(parameter.value);
            }
        }
    }

    [[nodiscard]] const Parameter<T>* const getParameter() const {
        return &parameter;
    }

private:
    Parameter<T> parameter;

    std::vector<IParameterView<T>*> views;
};

#endif // PARAMETER_H
