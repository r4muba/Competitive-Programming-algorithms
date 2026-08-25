class SparseTable {
public:
    SparseTable(const std::vector<int>& b) 
        : n {static_cast<int>(b.size())}, m{1}, lg(n + 1) {
        for (int i{2}; i <= n; ++i) lg[i] = lg[i / 2] + 1;
        m = lg[n] + 1;

        a.resize(m * n);
       
        for (int i{}; i < n; ++i) v(i, 0) = b[i];

        for (int j{1}; j < m; ++j) {
            for (int i{}; i + (1 << j) - 1 < n; ++i) {
                v(i, j) = std::min(v(i, j - 1), v(i + (1 << (j - 1)), j - 1));
            }
        }
    }
    int query(int l, int r) {
        --l; --r;
        int k {lg[r - l + 1]};
        return std::min(v(l, k), v(r - (1 << k) + 1, k));
    }
private:
    int n{}, m{};
    std::vector<int> a{}, lg{};
    int& v(int i, int j) { return a[(j * n) + i]; }
};
