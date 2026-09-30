const int INF {1'000'000'000};
std::vector<int> low(n), pre(n), id(n, -1);
std::stack<int> stk{};
stk.reserve(n);
int cont{}, scc{};

auto ldfs = [&](auto& dfs, int node) -> void { 
    pre[node] = low[node] = ++cont;
    stk.push(node);

    for (auto u : adj[node]) {
        if (pre[u] == 0) {
            dfs(dfs, u);
        }
        low[node] = std::min(low[node], low[u]);
    }
    if (low[node] == pre[node]) {
        while (true) {
            auto x{stk.top()}; stk.pop();
            id[x] = scc;
            low[x] = INF;
            if (x == node) break;
        }
        ++scc;
    }
};

for (int i{}; i < n; ++i) {
    if (pre[i] == 0) {
        dfs(dfs, i);
    }
}
