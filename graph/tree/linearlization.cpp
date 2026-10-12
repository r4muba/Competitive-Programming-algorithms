std::vector<int> node_in(n, -1), sz(n, 1);
std::vector<long long> flat(n);

int cont{};
auto ldfs = [&](auto& dfs, int node, int parent) -> int {
    node_in[node] = cont;
    flat[cont] = a[node];
    ++cont;

    for (auto u : adj[node]) {
        if (u == parent) continue;
        sz[node] += dfs(dfs, u, node);
    }
    return sz[node];
};

for (int i{}; i < n; ++i) {
    if (node_in[i] == -1) {
        ldfs(ldfs, i, -1);
    }
}
// query in flat tree [node_in[node], node_in[node] + sz[node] - 1];
