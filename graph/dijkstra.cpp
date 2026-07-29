#include <bits/stdc++.h>

const long long INF = 5e18;
std::vector<int> dis(n, INF);

using T = std::pair<long long, int>;
std::priority_queue<T, std::vector<T>, std::greater<T>> pq{};

dis[start] = 0;
pq.emplace(dis[start], start);

std::vector<std::vector<std::pair<int,int>>> path(n);

while (!pq.empty()) {
    auto [cost, node] {pq.top()}; pq.pop();
    if (cost > dis[node]) continue;
    for (auto [u, w] : adj[node]) {
        if (dis[node] + w < dis[u]) {
            dis[u] = dis[node] + w;
            pq.emplace(dis[u], u);
            path[u] = {std::pair{node, w}};
        } else if (dis[node] + w == dis[u]) {
            path[u].emplace_back(node, w);
        }
    }
}
