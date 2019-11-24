#include <fixed-point.h>

Fixed Multiply(Fixed x,Fixed y) {
    return ((int64_t)(x) * y / Precision);
}

Fixed Divide(Fixed x,Fixed y) {
    return ((int64_t)(x) * Precision / y);
}

Fixed InttoFixed(int x){ 
    return ((int32_t)(x) * Precision);
}

int FixedtoInt(Fixed x) {
    return ((x) / Precision);
}
