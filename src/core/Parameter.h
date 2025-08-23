#ifndef PARAMETER_H
#define PARAMETER_H

#include <string>

template<typename T>
struct Parameter {
    unsigned int id{};
    std::string name{};
    T min_value{};
    T max_value{};
    T default_value{};
    T value{};
};

#endif // PARAMETER_H
