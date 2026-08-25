const int mod = 1e9 + 7;
long long power(long long a, long long exp);

auto toMod(long long x) {
    return ((x % mod) + mod) % mod;
}
auto add(long long a, long long b) {
    auto ans {a + b};
    return (ans >= mod) ? ans - mod : ans;
}
auto sub(long long a, long long b) {
    auto ans {a - b};
    return (ans < 0) ? ans + mod : ans;
}
auto multi(long long a, long long b) {
    return (a * b) % mod;
}
auto divide(long long a, long long b) {
    return multi(a, power(b, mod - 2));
}
