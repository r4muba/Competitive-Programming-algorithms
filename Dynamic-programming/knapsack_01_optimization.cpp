/*  0/1 Knapsack optimization where sum of all items is ~ N
	There will be at most sqrt(N) different items.
	Array 'cnt' represents the count of a specific item.
*/

const long long maxn = 1e5+50;
const long long maxnsq = 400;

long long n,m,cnt[maxn],dp[maxnsq][maxn];
vector<long long> c;
void calculateDp(){ //DP in O(N*sqrt(N))
	dp[0][0]=0;
	for(int i = 1; i < n+5; i++) dp[0][i] = -1;
	for(int i = 1; i < (int)c.size(); i++){ // c is the array of unique items.
		for(int j = 1; j < n+1; j++){
			if(dp[i-1][j] >= 0)
				dp[i][j] = 0;
			else if(j-c[i] >= 0 && dp[i][j - c[i]] >= 0 and dp[i][j - c[i]] < cnt[c[i]])
				dp[i][j] = dp[i][j - c[i]] + 1;
			else
				dp[i][j] = -1;
		}
	}
}
