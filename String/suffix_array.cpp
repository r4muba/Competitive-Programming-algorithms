void csort(vector<long long>& sa, vector<long long>& r, int k){
	int n=sa.size();
	vector<long long> f(max(255,n),0),t(n);
	for(int i = 0; i < n; i++) f[(i+k<n?r[i+k]:0)]++;
	int sum=0;
	for(int i = 0; i < max(255,n); i++) f[i]=(sum+=f[i])-f[i];
	for(int i = 0; i < n; i++) t[f[(sa[i]+k<n?r[sa[i]+k]:0)]++]=sa[i];
	sa=t;
}
vector<long long> constructSA(string& s){ // O(nlogn)
	int n=s.size(),rank;
	vector<long long> sa(n),r(n),t(n);
	for(int i = 0; i < n; i++) sa[i]=i,r[i]=s[i];
	for(int k=1;k<n;k*=2){
		csort(sa,r,k);csort(sa,r,0);
		t[sa[0]]=rank=0;
		for(int i = 1; i < n; i++){
			if(r[sa[i]]!=r[sa[i-1]]||(sa[i]+k<n?r[sa[i]+k]:0)!=(sa[i-1]+k<n?r[sa[i-1]+k]:0))rank++;
			t[sa[i]]=rank;
		}
		r=t;
		if(r[sa[n-1]]==n-1)break;
	}
	return sa;
}
void doit(){ // Returns starting indices of all suffixes of the original string, sorted in lexicographical order.
	string s;
	cin>>s;
	s = "$" + s; // just in case, to avoid conflicts
	vector<long long> sa = constructSA(s);
}
