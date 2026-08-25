enum Colors {none, explored, visited};
std::vector<int> vis(n);
auto hasCycle = [&](auto& self, int node) -> bool {
    vis[node] = explored;
    for (auto u : adj[node]) {
        if (vis[u] == none) {
            if (self(self, u)) return true;
        } else if (vis[u] == explored) return true;
    }
    vis[node] = visited;
    return false;
};

// kanh algorithm
std::vector<int> q{};
q.reserve(n);
for (int i{}; i < n; ++i) {
    if (inDegree[i] == 0) {
        q.push_back(i);
    }
}
for (int j{}; j < std::ssize(q); ++j) {
    auto node {q[j]};
    for (auto u : adj[node]) {
        --inDegree[u];
        if (inDegree[u] == 0) {
            q.emplace_back(u);
        }
    }
}
if (std::ssize(q) != n) {
    std::cout << "There is a cycle.\n";
}
