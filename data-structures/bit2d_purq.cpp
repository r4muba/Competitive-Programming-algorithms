class BitPURQ2d {
public:
    BitPURQ2d (int rows, int cols) : n{rows + 1}, m{cols + 1}, v(n * m) {}
 
    long long query (int row_left, int col_left, int row_right, int col_right) {
        return sum(row_right, col_right) 
        - sum(row_left - 1, col_right) - sum(row_right, col_left - 1) 
        + sum(row_left - 1, col_left - 1);
    }
 
    void increment(int row, int col, long long delta) {
        for (int i{row}; i < n; i += lsb(i)) {
            for (int j{col}; j < m; j += lsb(j)) {
                ft(i, j) += delta;
            }
        }
    }
    
private:
    int n{}, m{};
    std::vector<long long> v{};

    long long& ft(int row, int col) { return v[(row * m) + col]; }
    int lsb (int i) { return (i & (-i)); }
 
    long long sum(int row, int col) {
        long long ans{};
        for (int i{row}; i; i -= lsb(i)) {
            for (int j{col}; j; j -= lsb(j)) {
                ans += ft(i, j);
            }
        }
        return ans;
    }
};
