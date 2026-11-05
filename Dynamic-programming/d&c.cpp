auto cost = [&](int l, int r) -> long long {
	return (prefix[r + 1] - prefix[l]) * (prefix[r + 1] - prefix[l]);
};

std::vector<long long> dp1(n), dp2(n);

auto ldp = [&](auto& dp, int l, int r, int optl, int optr) -> void {
    if (r < l) return;
    
    int mid {(l + r) / 2};
                                        // value, index
                                        // dp[corte][j - 1], corte
    std::pair<long long, int> optmedio{INF, -1};
    
    for (int i {optl}; i <= std::min(mid, optr); ++i) {
        long long my_cost = cost(i, mid);
        if (i) {
            my_cost += dp1[i - 1];
        }

        if (optmedio.first > my_cost) {
            optmedio = {my_cost, i};
        }
    }
    
    dp2[mid] = optmedio.first;
    
    dp(dp, l, mid - 1, optl, optmedio.second);
    dp(dp, mid + 1, r, optmedio.second, optr);
};

for (int i{}; i < k - 1; ++i) {
    ldp(ldp, 0, n - 1, 0, n - 1);
    std::swap(dp1, dp2);
}
