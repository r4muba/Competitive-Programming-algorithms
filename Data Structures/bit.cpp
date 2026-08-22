#include <bits/stdc++.h>
// Point-Update Range-Query
class BitPURQ {
public:
    BitPURQ (const std::vector<long long>& a) :
        n{std::ssize(a) + 1}, ft(n) {
            for (int i{1}; i < n; ++i) {
                ft[i] += a[i - 1];
                if (i + lsb(i) < n) ft [i + lsb(i)] += ft[i];
            }
        }
    auto sum(int i) {
        long long ans{};
        while (i) {
            ans += ft[i];
            i -= lsb(i);
        }
        return ans;
    }
    auto sumQuery(int l, int r) {
        return sum(r) - sum(l - 1);
    }
    void update(int i, long long delta) {
        while (i < n) {
            ft[i] += delta;
            i  += lsb(i);
        }
    }
    int find(long long k) {
        int i{}, b{1};
        while (b * 2 < n) b *= 2;
        for (int j{b}; j; j >>= 1) {
            if (i + j < n && ft[i + j] < k) {
                k -= ft[i + j];
                i += j;
            }
        }
        return i + 1;
    }
private:
    int n{};
    std::vector <long long> ft{};
    int lsb    (int i) { return i&(-i); }
};
// Range-Update Point-Query
class BitRUPQ {
public:
    BitRUPQ (const std::vector<long long>& a) 
        : n {static_cast<int>(a.size()) + 1}, ft(n) {
            for (int i{}; i < std::ssize(a); ++i) {
                increment(i + 1, i + 1, a[i]);
            }
        }
    auto value (int i) {
        long long ans{};
        while (i) {
            ans += ft[i];
            i -= lsb(i);
        }
        return ans;
    }
    void increment(int l, int r, long long delta) {
        update(l, delta);
        update(r + 1, -delta);
    }
private:
    int n{};
    std::vector<long long> ft{};
    int lsb(int i) { return (i&(-i)); }

    void update(int i, long long delta) {
        while (i < n) {
            ft[i] += delta;
            i += lsb(i);
        }
    }
};
