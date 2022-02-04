#ifndef VARIANTCONTAINER_H
#define VARIANTCONTAINER_H

#include <iostream>
#include <tuple>
#include <vector>
#include <core_global.h>

template<typename... Ts>
class CORE_LIBRARY_EXPORT VariantContainer
{
    template<typename T>
    using container = std::vector<T>;

public:
    template<typename T>
    void add(T e) {
        std::get<container<T>>(m_tupel).emplace_back(e);
    }

    template<typename T>
    container<T>& get() {
        return std::get<container<T>>(m_tupel);
    }

private:
    std::tuple<container<Ts>...> m_tupel;
};

#endif // VARIANTCONTAINER_H
