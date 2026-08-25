const int LOG {20}, INF {1'000'000'000};

std::vector<int> depth(n, INF);
depth[0] = 0;

std::vector<std::vector<int>> up(n, std::vector<int> (LOG, -1));
auto dfs = [&](auto& self, int node) -> void {
    for (auto u : adj[node]) {
        if (depth[u] != INF) continue;
        depth[u] = depth[node] + 1;
        up[u][0] = node;
        self(self, u);
    }
};
dfs(dfs, 0);
for (int k{1}; k < LOG; ++k) {
    for (int i{}; i < n; ++i) {
        int m {up[i][k - 1]};
        if (m != -1) {
            up[i][k] = up[m][k - 1];
        }
    }
}
auto lca = [&](int a, int b) {
    if (depth[a] < depth[b]) std::swap(a, b);
    int k{depth[a] - depth[b]};
    
    for (int i{}; i < LOG; ++i) {
        if (k & (1 << i)) {
            a = up[a][i];
        }
    }
    if (a == b) return a;
    for (int j{LOG - 1}; j >= 0; --j) {
        if (up[a][j] != up[b][j]) {
            a = up[a][j];
            b = up[b][j];
        }
    }
    return up[a][0];
}
