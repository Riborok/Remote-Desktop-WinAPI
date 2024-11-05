#pragma once

#include <type_traits>

class MathUtils {
public:
    MathUtils() = delete;

    template <typename T, typename U>
    static auto ceil(T numerator, U denominator) ->
            std::enable_if_t<std::is_integral_v<T> && std::is_integral_v<U>, std::common_type_t<T, U>> {
        return (numerator + denominator - 1) / denominator;
    }
};
