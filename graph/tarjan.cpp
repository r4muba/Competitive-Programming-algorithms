const long long maxn = 2e5+10;
long long n,x,y,m,timer=0;
long long low[maxn],disc[maxn],isArt[maxn],inStack[maxn];
vector<long long> adj[maxn];
vector<vector<long long>> scc;
vector < pair<long long, long long> > brid;
stack <int> stk; // to find SCCs (directed graphs)
vector <pair<long long, long long>> edges; // to find BCCs (undirected graphs)

void extract_bcc_nodes(int stop_u, int stop_v, vector<long long> &bcc_nodes){
	while((int)edges.size()){
	        auto e = edges.back();
	        edges.pop_back();
	        bcc_nodes.push_back(e.first);
	        bcc_nodes.push_back(e.second);
	        if ((e.first == stop_u && e.second == stop_v) || (e.first == stop_v && e.second == stop_u)) break;
	    }
	    sort(bcc_nodes.begin(), bcc_nodes.end());
	    bcc_nodes.erase(unique(bcc_nodes.begin(), bcc_nodes.end()), bcc_nodes.end());
}
 
void dfs(int node, int antnode = -1){
	low[node] = disc[node] = ++timer;
	stk.push(node);
	inStack[node] = 1;
	int children = 0;
	for (auto signode : adj[node]){
		if(disc[signode] == 0){
			children++;
			edges.push_back({node,signode});
			dfs(signode, node);

			// To find bridges:
			if (low[signode] > disc[node]){
				brid.push_back({node,signode});
			}

			low[node] = min(low[node], low[signode]);

			// To find articulation points (undirected graphs):
			if (antnode == -1 && children > 1) isArt[node] = 1;
			if (antnode != -1 && low[signode] >= disc[node]) isArt[node] = 1;

			// To find BCCs (undirected graphs):
			if (low[signode] >= disc[node]){
				vector<long long> bcc_nodes; // stores the nodes from a BCC (in no specific order)
				extract_bcc_nodes(node,signode,bcc_nodes);
			}
		}
		// To find SCC's (directed graphs):
		else if (inStack[signode] == 1){
			low[node] = min(low[node], disc[signode]);
		} 
		// To find articulation points, bridges, and/or BCCs (undirected graphs)
		else if (antnode != signode){
			if (disc[signode] < disc[node]){ // to find BCCs
				edges.push_back({node,signode});
			}
			low[node] = min(low[node], disc[signode]);
		} 
	}
	if (low[node] == disc[node]){ // To find SCC's (directed graphs)
		vector<long long> scctem;
		while (true){
			long long topic = stk.top();
			stk.pop();
			scctem.push_back(topic);
			inStack[topic] = 0;
			if (node == topic) break;
		}
		scc.push_back(scctem);
	}
}
