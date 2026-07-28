#include <bits/stdc++.h>

std::vector<long long> primes{};
const int nax {100'000};

const auto gc = []() {
    std::bitset<nax + 1> c{};
    c[0] = c[1] = true;
    primes.reserve((nax / std::log(nax)) * 1.2);

    for (int m{4}; m <= nax; m += 2) c[m] = true;
    primes.push_back(2);

    for (long long p{3}; p <= nax; p += 2) {
        if (c[p]) continue;
        primes.push_back(p);
        for (long long m{p * p}; m <= nax; m += p + p) {
            c[m] = true;
        }
    }
    return c;
}();

bool isPrime(long long m) {
    if (m <= nax) return !gc[m];
    for (auto p : primes) {
        if (p * p > m) break;
        if (m % p == 0) return false;
    }
    return true;
}

auto primeFactors(long long m) {
    std::vector<std::pair<long long,int>> f{};
    for (auto p : primes) {
        if (p * p > m) break;
        int cont{};
        while (m % p == 0) {
            ++cont;
            m /= p;
        }
        if (cont) f.emplace_back(p, cont);
    }
    if (m != 1) f.emplace_back(m, 1);
    return f;
}

auto eulerPhiSieve = []() {
    std::array <long long, nax + 1> a{};
    std::iota(a.begin(), a.end(), 0);
    for (int i{2}; i <= nax; ++i) {
        if (a[i] == i) {
            for (int j{i}; j <= nax; j += i) {
                a[j] = (a[j] / i) * (i - 1);
            }
        }
    }
    return a;
}();

auto eulerPhi(long long n) {
    if (n <= nax) return eulerPhiSieve[n];
    long long ans{n};
    for (auto p : primes) {
        if (p * p > n) break;
        if (n % p == 0) ans -= ans / p;
        while (n % p == 0) n /= p;
    }
    if (n != 1) ans -= ans / n;
    return ans;
}
