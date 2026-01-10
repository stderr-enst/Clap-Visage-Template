#ifndef IPARAMETERVIEW_H
#define IPARAMETERVIEW_H

template<typename T>
class IParameterView {
public:
    virtual ~IParameterView() = default;

    virtual void setView(const T& v) = 0;
    [[nodiscard]] virtual const T getDisplayValue() const = 0;
};

#endif // IPARAMETERVIEW_H
