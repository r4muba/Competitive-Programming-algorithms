// Kruskal
std::vector<std::tuple<int,int,int>> edges{};
std::ranges::sort(edges);

UF dsu{n};
std::vector<std::vector<int>> adj(n);
for (auto [weight, u, v] : edges) {
    if (dsu.unite(u, v)) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    if (dsu.components() == 1) break;
}

// PRIM
using T = std::pair<int, int>;
std::priority_queue<T, std::vector<T>, std::greater<T>> pq{};
std::vector<int> vis(n);
auto process = [&](int node) {
    vis[node] = true;
    for (auto [u, w] : adj[node]) {
        if (vis[u]) continue;
        pq.emplace(w, u);
    }
};
process(0);
long long cost{};
while (!pq.empty()) {
    auto [w, node] {pq.top()}; pq.pop();
    if (vis[node]) continue;
    cost += w;
    process(node);
}
