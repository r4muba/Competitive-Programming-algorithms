const long long inf = 1e18+7;

struct FlowGraph {
 
	struct Edge {
		long long to, flow, cap, cost;
		Edge *res;
 
		Edge () : to(0), flow(0), cap(0), cost(0), res(0) {}
		Edge (long long to, long long flow, long long cap, long long cost) : to(to), flow(flow), cap(cap), cost(cost), res(0) {}
	
		void addFlow (long long f) {
			flow += f;
			res->flow -= f;
		}
	};
 
	 long long n;
	vector<vector<Edge*>> adj;
	vector<long long> dis, pos;
	
 
	FlowGraph (int n) : n(n), adj(n), dis(n), pos(n) {} 
 
	void add (int u, int v, long long cap, long long cost) {
		Edge *x = new Edge(v, 0, cap, cost);
		Edge *y = new Edge(u, cap, cap, -cost);
		x->res = y;
		y->res = x;
		adj[u].push_back(x);
		adj[v].push_back(y);
	}

	pair<long long, long long> mcmf(int s, int t, long long tope) {
		vector<bool> inq(n);
		vector<long long> dis(n), cap(n);
		vector<Edge*> par(n);
		long long maxFlow = 0, minCost = 0;
 
		while (maxFlow < tope) { //  compute MCF: maxflow < tope, compute MCMF maxflow < inf 
			fill(dis.begin(), dis.end(), inf);
			fill(par.begin(), par.end(), nullptr);
			fill(cap.begin(), cap.end(), 0);
			dis[s] = 0;
			cap[s] = inf;
			queue<int> q;
			q.push(s);
 
			while ((int)q.size()) {
				int u = q.front();
				q.pop();
				inq[u] = 0;
 
				for (Edge *v : adj[u]) {
					if (v->cap > v->flow && dis[v->to] > dis[u] + v->cost) {
						dis[v->to] = dis[u] + v->cost;
						par[v->to] = v;
						cap[v->to] = min(cap[u], v->cap - v->flow);
						
						if (!inq[v->to]) {
							q.push(v->to);
							inq[v->to] = 1;
						}
					}
				}
			}
 
			if (!par[t]) break;
 
			maxFlow += cap[t];
			minCost += cap[t] * dis[t];
			for (int u = t; u != s; u = par[u]->res->to)
				par[u]->addFlow(cap[t]);
		}
 
		return {maxFlow, minCost};
	}
};

void doit(){
	// define src and sink.
	// edges src to node, and node to sink have cost 0.
	// to compute flow matches (e.g assignment problems), run dfs over the flow graph, keep the path and substract one unit of flow every time.
}
