#include <bits/stdc++.h>
int n{}, m{};

// Undirected edges {vertex, id}
std::vector<std::vector<std::pair<int,int>>> adj(n);

std::vector<int> bridges(m), articulation(n);
int root{}, childrenRoot{};

std::vector<int> pre(n), low(n);
int cont{};
auto dfs = [&](auto& self, int node, int parentEdge) -> void {
    pre[node] = low[node] = ++cont;
    for (auto [u, id] : adj[node]) {
        if (id == parentEdge) continue;
        if (pre[u] == 0) {
            // forward/tree edge
            self(self, u, id);
            if (node == root) ++childrenRoot;
            if (low[u] > pre[node]) bridges[id] = true;
            if (low[u] >= pre[node]) articulation[node] = true;
            low[node] = std::min(low[u], low[node]);
        } else {
            // back edge
            low[node] = std::min(low[node], pre[u]);
        }
    }
};

for (int i{}; i < n; ++i) {
    if (pre[i]) continue;
    root = i; childrenRoot = 0;
    dfs(dfs, i, -1);
    articulation[i] = (childrenRoot > 1);
}
