int J(int n, int k) {
    if (n == 1) return 0; // 1 (1-index)
    return (J(n - 1, k) + k) % n;
}

int josephus(int n, int k) {
    int res = 0;
    for (int i = 1; i <= n; ++i){
        res = (res + k) % i;
    }
    return res; // + 1 (1-index)
}

std::vector<int> ans{};
int pos{1};
// 1-index
for (int i{}; i < n; ++i) {
    int r {n - i};
    pos = ((pos + k - 1) % r) + 1;
    ans.push_back(jose.find(pos));
    jose.kill(ans.back());
}
