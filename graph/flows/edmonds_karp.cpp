template <int N>
class Graph {
public:
    Graph(int src, int snk) 
        : source{src}, sink{snk} {
            q.reserve(N);
        }

    void add(int u, int v, long long w) {
        cap[u][v] += w;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    long long max_flow() {
        long long ans{};

        while (true) {
            auto flow {bfs()};
            if (flow == 0) break;
            ans += flow;

            int cur {sink};
            while (cur != source) {
                int prev {p[cur]};
                cap[prev][cur] -= flow;
                cap[cur][prev] += flow;
                cur = prev;
            }
        }
        return ans;
    }

private:
    int source{}, sink{};
    std::array< std::vector<int>, N> adj{};
    std::array< std::array<long long, N>, N> cap{};
    std::array<int, N> p{};
    std::vector< std::pair<int, long long>> q{};

    long long bfs() {
        std::ranges::fill(p, -1);
        p[source] = -2;
        
        q.clear();
        q.emplace_back(source, INF);

        for (int j{}; j < std::ssize(q); ++j) {
            auto [node, flow] {q[j]};
            for (auto u : adj[node]) {
                if (p[u] != -1 || !cap[node][u]) continue;
                p[u] = node;

                auto x { std::min(flow, cap[node][u]) };
                if (u == sink) return x;
                q.emplace_back(u, x);
            }
        }
        return 0;
    }
};
