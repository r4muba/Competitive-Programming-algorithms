#include <bits/stdc++.h>

dis[start] = 0;
for (int i{}; i < n - 1; ++i) {
    for (int node{}; node < n; ++node) {
        if (dis[node] == INF) continue;
        for (auto [u, w] : adj[node]) {
            dis[u] = std::min(dis[u], dis[node] + w);
        }
    }
}
std::vector<int> ciclosos(n), q{};
for (int node{}; node < n; ++node) {
    if (dis[node] == INF) continue;
    for (auto [u, w] : adj[node]) {
        if (dis[node] + w < dis[u]) {
            ciclosos[u] = true;
            q.push_back(u);
        }
    }
}
for (int j{}; j < std::ssize(q); ++j) {
    auto node {q[i]};
    for (auto [u, w] : adj[node]) {
        if (ciclosos[u]) continue;
        ciclosos[u] = true;
        q.push_back(u);
    }
}
