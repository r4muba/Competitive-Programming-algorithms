// An Eulerian path is a path that goes exactly once through each edge of the graph.
// An Eulerian circuit is an Eulerian path that starts and ends at the same node.

// Undirected graph has an Eulerian path exactly when all the edges
// belong to the same connected component and
// - the degree of each node is even 
// or
// - the degree of exactly two nodes is odd, and the degree of all other nodes is
// even.

// Directed graph contains an Eulerian path exactly when all the edges belong to
// the same connected component and
// - in each node, the indegree equals the outdegree, 
// or
// - in one node, the indegree is one larger than the outdegree, in another node,
// the outdegree is one larger than the indegree, and in all other nodes, the
// indegree equals the outdegree.

auto hierholzer = [&](int start) {
    std::vector<int> ans{}, vis(m), ptr(n);
    std::vector<int> stk{start};
    ans.reserve(n);

    while (!stk.empty()) {
        auto node {stk.back()};
        int& i {ptr[node]};

        while ( i < std::ssize(adj[node]) && vis[adj[node][i].second]) {
            ++i;
        }
        if (i < std::ssize(adj[node])) {
            auto& [u, id] {adj[node][i]};
            vis[id] = true;
            stk.push_back(u);
            ++i;
        } else {
            ans.push_back(node);
            stk.pop_back();
        }
    }
    std::ranges::reverse(ans);
    return ans;
};
