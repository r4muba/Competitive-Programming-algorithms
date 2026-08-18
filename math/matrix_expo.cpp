const long long mod = 1e9+7;
long long tc,n,m,k;

vector<vector<long long>> mul(vector<vector<long long>> a, vector<vector<long long>> b) {
	vector<vector<long long>> c((int)a.size(), vector<long long>((int)b[0].size()));
	for (int i = 0; i < (int)a.size(); i++) 
		for (int j = 0; j < (int)b.size(); j++)
			for ( int k = 0; k < (int)a.size(); k++) 
				(c[i][j] += a[i][k]*b[k][j]%mod)%=mod; //for amount of paths.
				//c[i][j] = min(c[i][j], a[i][k] + b[k][j]); //for shortest path.
	return c;
}
 
vector<vector<long long>> exp( vector<vector<long long>> x, int y) { // matrix and desired power.
	vector<vector<long long>> r((int)x.size(), vector<long long>((int)x.size(),0ll)); //0ll: amount of paths. INF: shortest path
	for ( int i = 0; i < (int)x.size(); i++) r[i][i] = 1; //1ll: amount of paths. 0ll: shortest path.
	while (y>0){
		if (y&1) {
			r = mul(r,x);
		}
		y=y>>1;
		x = mul(x,x);
	}
	return r;
}

void doit(){
	// build adjacency (or costs) matrix of size(n*n).
	// after exponentiating mat[i][j] denotes the path from i to j.
}
