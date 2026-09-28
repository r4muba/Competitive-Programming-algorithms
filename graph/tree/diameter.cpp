const int INF {1'000'000'000};
std::vector<int> depth(n, INF);

auto diameter = [&](int start) -> int {
    std::vector<int> q{start};
    depth[start] = 0;

    for (int i{}; i < std::ssize(q); ++i) {
        auto node {q[i]};
        for (auto u : adj[node]) {
            if (depth[node] > depth[u]) continue;
            depth[u] = depth[node] + 1;
            q.push_back(u);
        }
    }
    
    int ans {};
    auto ldp = [&](auto& dfs, int node, int parent) -> int {
        int max{};

        for (auto u : adj[node]) {
            if (parent == u) continue;

            int x = dfs(dfs, u, node);
            ans = std::max(ans, max + x + 1);
            max = std::max(max, x + 1);
        }
        return max;
    };
    ldp(ldp, start, -1);
    return ans;
};
