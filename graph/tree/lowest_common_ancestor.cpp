const int nax {200'005}, LOG {20}, INF {1'000'000'000};
static std::array< std::array<int, LOG>, nax> up{};
std::vector<int> depth(n, INF);

std::vector<int> q{0};
q.reserve(n);
depth[0] = 0;

for (int j{}; j < std::ssize(q); ++j) {
    auto node {q[j]};
    for (auto u : adj[node]) {
        if (depth[node] > depth[u]) continue;
        up[u][0] = node;
        depth[u] = depth[node] + 1;
        q.push_back(u);
    }
}

for (int j{1}; j < LOG; ++j) {
    for (int i{}; i < n; ++i) {
        up[i][j] = up[up[i][j - 1]][j - 1];
    }
}

auto walk  = [&](int node, int k) -> int {
    if (k > depth[node]) return -1;
    for (int i{}; i < LOG; ++i) {
        if (k & (1 << i)) {
            node = up[node][i];
        }
    }
    return node;
};

auto lca = [&](int x, int y) -> int {
    if (depth[x] < depth[y]) std::swap(x, y);
    x = walk(x, depth[x] - depth[y]);
    if (x == y) return x;
    for (int i{LOG - 1}; i >= 0; --i) {
        if (up[x][i] != up[y][i]) {
            x = up[x][i];
            y = up[y][i];
        }
    }
    return up[x][0];
};
