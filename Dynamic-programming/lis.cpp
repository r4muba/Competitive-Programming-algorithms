// Longest increasing subsequence O(nlogn)
const long long INF = 1e18;
int lis(const vector<long long> &a) {
	int n = (int)a.size();
	vector<long long> d(n+1, INF);
	d[0] = -INF;

	for (int i = 0; i < n; i++) {
		int j = upper_bound(d.begin(), d.end(), a[i]) - d.begin();
		if (d[j-1] < a[i] && a[i] < d[j]) d[j] = a[i];
	}

	int ans = 0;
	for(int i = 0; i < n+1; i++) if (d[i]<INF) ans = i;
	return ans;
}
