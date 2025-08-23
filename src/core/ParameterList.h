#ifndef PARAMETERLIST_H
#define PARAMETERLIST_H

#include "Parameter.h"

#include <algorithm>
#include <concepts>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

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

        return nextid;
    }

    std::optional<const unsigned int> getParameterId(const std::string lookup) const {
        if (nameToId.contains(lookup)) {
            return nameToId.at(lookup);
        }

        return {};
    }

    const std::string getParameterName(const unsigned int id) const {
        if (idInParameters(id)) {
            return parameters[id].name;
        }

        return std::string("");
    }

    const T getParameterValue(const unsigned int id) const {
        if (idInParameters(id)) {
            return parameters[id].value;
        }

        return T{0};
    }

    const T getParameterMin(const unsigned int id) const {
        if (idInParameters(id)) {
            return parameters[id].min_value;
        }

        return T{0};
    }

    const T getParameterMax(const unsigned int id) const {
        if (idInParameters(id)) {
            return parameters[id].max_value;
        }

        return T{0};
    }

    const T getParameterDefault(const unsigned int id) const {
        if (idInParameters(id)) {
            return parameters[id].default_value;
        }

        return T{0};
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

    void setParameterMin(const unsigned int id, const T& newmin) {
        if (idInParameters(id)) {
            parameters[id].min_value = newmin;
        }
    }

    void setParameterMax(const unsigned int id, const T& newmax) {
        if (idInParameters(id)) {
            parameters[id].max_value = newmax;
        }
    }

    void setParameterDefault(const unsigned int id, const T& newdefault) {
        if (idInParameters(id)) {
            parameters[id].default_value = newdefault;
        }
    }

    void setParameterToDefault(const unsigned int id) {
        if (idInParameters(id)) {
            parameters[id].value = parameters[id].default_value;
        }
    }

private:
    bool idInParameters(const unsigned int id) const {
        return id < parameters.size();
    }

    std::unordered_map<std::string, unsigned int> nameToId{};
    std::vector<Parameter<T>> parameters{};
};

#endif // PARAMETERLIST_H
