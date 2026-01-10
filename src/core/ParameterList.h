#ifndef PARAMETERLIST_H
#define PARAMETERLIST_H

#include "IParameterView.h"

#include <algorithm>
#include <concepts>
#include <optional>
#include <string>
#include <unordered_map>
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

template<typename T> requires std::totally_ordered<T>
class ParameterList {
public:
    ParameterList() {
        parameters.reserve(100);
    };

    const unsigned int createParameter(
              std::string newname,
              T min,
              T max,
              T def
            ) {
        const unsigned int nextid = parameters.size();

        Parameter<T> newparam = {
            .id = nextid,
            .name = newname,
            .min_value = min,
            .max_value = max,
            .default_value = def,
            .value = def
        };
        parameters.emplace_back(newparam);
        nameToId.emplace(newname, nextid);
        views.emplace(newparam.id, std::vector<IParameterView<T>*>{});

        return nextid;
    }

    std::optional<const unsigned int> getParameterId(const std::string& lookup) const {
        if (nameToId.contains(lookup)) {
            return nameToId.at(lookup);
        }

        return {};
    }

    std::optional<const std::string> getParameterName(const unsigned int id) const {
        if (idInParameters(id)) {
            return parameters[id].name;
        }

        return {};
    }

    std::optional<const Parameter<T>* const> getParameter(const unsigned int id) const {
        if (idInParameters(id)) {
            return &parameters[id];
        }

        return {};
    }

    std::optional<const Parameter<T>* const> getParameter(const std::string& lookup) const {
        auto idoptional = getParameterId(lookup);
        if (idoptional) {
            return getParameter(idoptional.value());
        }

        return {};
    }


    std::optional<const T> getParameterValue(const unsigned int id) const {
        if (idInParameters(id)) {
            return parameters[id].value;
        }

        return {};
    }

    std::optional<const T> getParameterValue(const std::string& lookup) const {
        auto idoptional = getParameterId(lookup);
        if (idoptional) {
            return getParameterValue(idoptional.value());
        }

        return {};
    }

    std::optional<const T> getParameterMin(const unsigned int id) const {
        if (idInParameters(id)) {
            return parameters[id].min_value;
        }

        return {};
    }

    std::optional<const T> getParameterMin(const std::string& lookup) const {
        auto idoptional = getParameterId(lookup);
        if (idoptional) {
            return getParameterMin(idoptional.value());
        }

        return {};
    }

    std::optional<const T> getParameterMax(const unsigned int id) const {
        if (idInParameters(id)) {
            return parameters[id].max_value;
        }

        return {};
    }

    std::optional<const T> getParameterMax(const std::string& lookup) const {
        auto idoptional = getParameterId(lookup);
        if (idoptional) {
            return getParameterMax(idoptional.value());
        }

        return {};
    }

    std::optional<const T> getParameterDefault(const unsigned int id) const {
        if (idInParameters(id)) {
            return parameters[id].default_value;
        }

        return {};
    }

    std::optional<const T> getParameterDefault(const std::string& lookup) const {
        auto idoptional = getParameterId(lookup);
        if (idoptional) {
            return getParameterDefault(idoptional.value());
        }

        return {};
    }

    void setParameterName(const unsigned int id, const std::string newname) {
        if (idInParameters(id)) {
            parameters[id].name = newname;
        }
    }

    void setParameterValue(const unsigned int id, const T& newvalue) {
        if (idInParameters(id)) {
            const T clampedValue = std::clamp(newvalue, parameters[id].min_value, parameters[id].max_value);
            parameters[id].value = clampedValue;
        }
    }

    void setParameterValue(const std::string& lookup, const T& newvalue) {
        auto idoptional = getParameterId(lookup);
        if (idoptional) {
            setParameterValue(idoptional.value(), newvalue);
        }
    }

    void setParameterMin(const unsigned int id, const T& newmin) {
        if (idInParameters(id)) {
            parameters[id].min_value = newmin;
        }
    }

    void setParameterMin(const std::string& lookup, const T& newvalue) {
        auto idoptional = getParameterId(lookup);
        if (idoptional) {
            setParameterMin(idoptional.value(), newvalue);
        }
    }

    void setParameterMax(const unsigned int id, const T& newmax) {
        if (idInParameters(id)) {
            parameters[id].max_value = newmax;
        }
    }

    void setParameterMax(const std::string& lookup, const T& newvalue) {
        auto idoptional = getParameterId(lookup);
        if (idoptional) {
            setParameterMax(idoptional.value(), newvalue);
        }
    }

    void setParameterDefault(const unsigned int id, const T& newdefault) {
        if (idInParameters(id)) {
            parameters[id].default_value = newdefault;
        }
    }

    void setParameterDefault(const std::string& lookup, const T& newvalue) {
        auto idoptional = getParameterId(lookup);
        if (idoptional) {
            setParameterDefault(idoptional.value(), newvalue);
        }
    }

    void setParameterToDefault(const unsigned int id) {
        if (idInParameters(id)) {
            parameters[id].value = parameters[id].default_value;
        }
    }

    unsigned int size() {
        return parameters.size();
    }

    void registerView(const std::string& lookup , IParameterView<T>* view) {
        auto idoptional = getParameterId(lookup);
        if (idoptional) {
            registerView(idoptional.value(), view);
        }
    }

    void registerView(unsigned int id, IParameterView<T>* view) {
        if (view != nullptr) {
            views[id].push_back(view);
        }

        updateViews(id);
    }

    void updateViews(unsigned int id) {
        for (auto view : views[id]) {
            if (view != nullptr) {
                view->setView(parameters[id].value);
            }
        }
    }

    void updateAllViews() {
        for (auto param : parameters) {
            for (auto view : views[param.id]) {
                if (view != nullptr) {
                    view->setView(param.value);
                }
            }
        }
    }

private:
    bool idInParameters(const unsigned int id) const {
        return id < parameters.size();
    }

    std::unordered_map<std::string, unsigned int> nameToId{};
    std::vector<Parameter<T>> parameters{};
    std::unordered_map<unsigned int, std::vector<IParameterView<T>*>> views;
};

#endif // PARAMETERLIST_H
