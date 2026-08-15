vector<long long> prefix_function(string s){
    long long n= (int)s.size();
    vector<long long> pi(n);

    for(int i = 1; i<n; i++){
        long long j=pi[i-1];

        while(j>0 && s[i]!=s[j])
            j=pi[j-1];

        if(s[i]==s[j])
            j++;

        pi[i]=j;
    }

    return pi;
}

vector<long long> kmp(string s,string pat){
    vector<long long> pi=prefix_function(pat);
    vector<long long> ans;

    long long j=0;

    for(int i = 0; i< (int)s.size(); i++){
        while(j>0 && s[i]!=pat[j])
            j=pi[j-1];

        if(s[i]==pat[j])
            j++;

        if(j==sz(pat)){
            ans.pb(i-sz(pat)+1);
            j=pi[j-1];
        }
    }

    return ans;
}
