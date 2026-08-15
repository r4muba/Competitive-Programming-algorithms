#include <bits/stdc++.h>

using namespace std;

using ll = long long;

struct FlowEdge {
    int from;
    int to;
    ll cap;
    ll flow;

    FlowEdge(int from, int to, ll cap)
        : from(from), to(to), cap(cap), flow(0) {}
};

struct Dinic {
    const ll INF = (ll)4e18;

    int n;
    int source;
    int sink;

    vector<FlowEdge> edges;
    vector<vector<int>> adj;

    vector<int> level;
    vector<int> ptr;

    Dinic(int n, int source, int sink)
        : n(n), source(source), sink(sink) {

        adj.resize(n);
        level.resize(n);
        ptr.resize(n);
    }

    // Directed edge: from -> to with capacity cap
    void add_edge(int from, int to, ll cap) {
        int id = (int)edges.size();

        // Original edge
        edges.emplace_back(from, to, cap);

        // Residual edge
        edges.emplace_back(to, from, 0);

        adj[from].push_back(id);
        adj[to].push_back(id + 1);
    }

    // Builds the level graph
    bool bfs() {
        fill(level.begin(), level.end(), -1);

        queue<int> q;

        level[source] = 0;
        q.push(source);

        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (int id : adj[v]) {
                FlowEdge& e = edges[id];

                // No residual capacity
                if (e.cap - e.flow <= 0) {
                    continue;
                }

                // Already visited
                if (level[e.to] != -1) {
                    continue;
                }

                level[e.to] = level[v] + 1;
                q.push(e.to);
            }
        }

        return level[sink] != -1;
    }

    // Sends flow through the level graph
    ll dfs(int v, ll pushed) {
        if (pushed == 0) {
            return 0;
        }

        if (v == sink) {
            return pushed;
        }

        for (int& cid = ptr[v];
             cid < (int)adj[v].size();
             ++cid) {

            int id = adj[v][cid];
            FlowEdge& e = edges[id];

            if (level[e.to] != level[v] + 1) {
                continue;
            }

            if (e.cap - e.flow <= 0) {
                continue;
            }

            ll tr = dfs(
                e.to,
                min(pushed, e.cap - e.flow)
            );

            if (tr == 0) {
                continue;
            }

            e.flow += tr;

            // Reverse edge
            edges[id ^ 1].flow -= tr;

            return tr;
        }

        return 0;
    }

    // Returns maximum flow
    ll max_flow() {
        ll totalFlow = 0;

        while (bfs()) {
            fill(ptr.begin(), ptr.end(), 0);

            while (true) {
                ll pushed = dfs(source, INF);

                if (pushed == 0) {
                    break;
                }

                totalFlow += pushed;
            }
        }

        return totalFlow;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    /*
        BASIC USAGE
        -----------

        Suppose we have nodes:

            0, 1, 2, 3

        plus:

            source = 4
            sink   = 5

        Therefore:

            totalNodes = 6
    */

    int totalNodes = 6;
    int source = 4;
    int sink = 5;

    Dinic dinic(totalNodes, source, sink);

    /*
        Add directed edges:

        add_edge(from, to, capacity)
    */

    dinic.add_edge(source, 0, 3);
    dinic.add_edge(source, 1, 2);

    dinic.add_edge(0, 2, 2);
    dinic.add_edge(0, 3, 1);
    dinic.add_edge(1, 2, 1);
    dinic.add_edge(1, 3, 2);

    dinic.add_edge(2, sink, 2);
    dinic.add_edge(3, sink, 3);

    /*
        Run Dinic.
    */

    ll flow = dinic.max_flow();

    cout << "Maximum flow: " << flow << '\n';


    /*
        ---------------------------------------
        HOW TO INSPECT THE FLOW OF EVERY EDGE
        ---------------------------------------

        Every add_edge() adds TWO edges:

            edges[id]     = original
            edges[id ^ 1] = residual

        Therefore, original edges are:

            0, 2, 4, 6, ...

        You can recover which edges were used by
        checking:

            e.flow > 0
    */

    for (int id = 0; id < (int)dinic.edges.size(); id += 2) {
        FlowEdge& e = dinic.edges[id];

        cout
            << e.from
            << " -> "
            << e.to
            << "   flow = "
            << e.flow
            << "/"
            << e.cap
            << '\n';
    }


    /*
        ---------------------------------------
        BIPARTITE MATCHING
        ---------------------------------------

        If you have:

            Left side:
                0 ... L - 1

            Right side:
                L ... L + R - 1

            source = L + R
            sink   = L + R + 1

        then:

            Dinic dinic(L + R + 2, source, sink);

        source -> LEFT:
            capacity 1

        LEFT -> RIGHT:
            capacity 1

        RIGHT -> sink:
            capacity 1


        Example:

            for (int i = 0; i < L; ++i) {
                dinic.add_edge(source, i, 1);
            }

            for (int j = 0; j < R; ++j) {
                dinic.add_edge(L + j, sink, 1);
            }

            if (left_i can match right_j) {
                dinic.add_edge(i, L + j, 1);
            }

            ll matching = dinic.max_flow();


        ---------------------------------------
        RECOVER MATCHES
        ---------------------------------------

        for (int id = 0;
             id < (int)dinic.edges.size();
             id += 2) {

            FlowEdge& e = dinic.edges[id];

            bool fromLeft =
                e.from >= 0 &&
                e.from < L;

            bool toRight =
                e.to >= L &&
                e.to < L + R;

            if (fromLeft && toRight && e.flow == 1) {

                int left = e.from;
                int right = e.to - L;

                cout << left << " " << right << '\n';
            }
        }


        ---------------------------------------
        B-MATCHING
        ---------------------------------------

        If every node can be matched K times,
        just change the capacities:

            source -> left    capacity K
            right  -> sink    capacity K

        while:

            left -> right     capacity 1

        Example for degree <= 2:

            dinic.add_edge(source, left, 2);
            dinic.add_edge(right, sink, 2);

        The reconstruction is exactly the same:
        left -> right edges with flow == 1.
    */

    return 0;
}
