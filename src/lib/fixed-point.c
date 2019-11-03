#include <fixed-point.h>

Fixed Add(Fixed x, int n) {
    return x + InttoFixed(n);
}

Fixed Subtract(Fixed x, int n) {
    return x - InttoFixed(n);
}

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