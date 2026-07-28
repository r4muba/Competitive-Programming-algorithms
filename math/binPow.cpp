const int mod = 1e9 + 7;

auto power (long long a, long long exp) {
    long long r{1};
    while (exp) {
        if (exp % 2) {
            r = (a * r) % mod;
        }
        a = (a * a) % mod;
        exp /= 2;
    }
    return r;
}

