#include <bits/stdc++.h>

class Bit {
public:
    Bit (std::vector<long long>& a) :
        n{std::ssize(a) + 1}, ft(n) {
            for (int i{1}; i < n; ++i) {
                ft[i] += a[i - 1];
                if (parent(i) < n) ft[parent(i)] += ft[i];
            }
        }
    auto sum(int i) {
        long long ans{};
        while (i) {
            ans += ft[i];
            i = child(i);
        }
        return ans;
    }
    auto sumQuery(int l, int r) {
        return sum(r) - sum(l - 1);
    }
    void update(int i, long long delta) {
        while (i < n) {
            ft[i] += delta;
            i = parent(i);
        }
    }
    int find(long long k) {
        int i{};
        int b = std::bit_floor(static_cast<unsigned int>(n));
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
    int parent (int i) { return i + lsb(i); }
    int child  (int i) { return i - lsb(i); }
};
