#include <bits/stdc++.h>

constexpr double eps {1e-9};

bool equal(double a, double b) {
    if (std::abs(a - b) <= eps) return true;
    return std::abs(a - b) <= std::max(std::abs(a), std::abs(b)) * eps;
}
bool less(double a, double b) {
    return a < (b - eps);
}
bool lessEqual(double a, double b) {
    return a < (b + eps);
}
long long divCeil(long long x, long long y) {
    bool neg {(x < 0) != (y < 0)};
    if (neg) return x / y;
    return (x / y) + (x % y != 0);
}
