class BitRUPQ {
public:
    BitRUPQ (const std::vector<long long>& a) 
        : n{static_cast<int>(a.size()) + 1}, ft(n) {
            for (int i{}; i < std::ssize(a); ++i) {
                increment(i + 1, i + 1, a[i]);
            }
        }
    long long value (int i) {
        long long ans{};
        while (i) {
            ans += ft[i];
            i -= lsb(i);
        }
        return ans;
    }
    void increment(int l, int r, long long delta) {
        sum(l, delta);
        sum(r + 1, -delta);
    }
private:
    int n{};
    std::vector<long long> ft{};
    int lsb(int i) { return (i&(-i)); }

    void sum(int i, long long delta) {
        while (i < n) {
            ft[i] += delta;
            i += lsb(i);
        }
    }
};
