#include <bits/stdc++.h>

long long power(long long a, long long b);
long long multi(long long a, long long b);
long long divide(long long a, long long b);


const int nax {67};
auto pascal = []() {
    std::array< std::array<long long, nax>, nax> p{};
    p[0][0] = 1;
    for (int i{1}; i < nax; ++i) {
        p[i][0] = 1;
        for (int j{1}; j < nax; ++j) {
            p[i][j] += p[i - 1][j] + p[i - 1][j - 1];
        }
    }
    return p;
}();

const int nax {100'000};

std::vector<long long> invFacto(nax, 1);
const auto facto = []() {
    std::array<long long, nax> f;
    f[0] = 1;
    for (int i{1}; i < nax; ++i) {
        f[i] = multi(f[i - 1], i);
    }
    invFacto[nax - 1] = divide(1, facto[nax - 1]);
    for (int i{nax - 1}; i > 0; --i) {
        invFacto[i - 1] = multi(invFacto[i], i);
    }
    return f;
}();

long long nCk(int n, int k) {
    if (k < 0 || n < k) return 0;
    return pascal[n][k];
    return multi(facto[n], multi(invFacto[k], invFacto[n - k]));
}
