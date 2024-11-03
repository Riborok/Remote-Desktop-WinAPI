#include "../../inc/utils/MathUtils.hpp"

size_t MathUtils::ceil(const size_t numerator, const size_t denominator) {
    return (numerator + denominator - 1) / denominator;
}
