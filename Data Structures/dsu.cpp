class UF {
public:
    UF (int size) 
        : n{size}, c{n}, p(n), sz(n, 1) {
            std::iota(p.begin(), p.end(), 0);
        }
    int find (int i) {
        while (p[i] != i) i = p[i] = p[p[i]];
        return i;
    }
    bool same(int i, int j) { 
        return find(i) == find(j); 
    }
    bool unite(int i, int j) {
        i = find(i); j = find(j);
        if (i == j) return false;
        if (sz[i] < sz[j]) std::swap(i, j);
        p[j] = i;
        sz[i] += sz[j];
        --c;
        return true;
    }
    int size(int i) { return sz[find(i)]; }
    int components() { return c; }
private:
    int n{}, c{};
    std::vector<int> p{}, sz{};
};
