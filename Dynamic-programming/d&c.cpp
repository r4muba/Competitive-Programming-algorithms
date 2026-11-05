int curl{}, curr{-1};
long long cur_total{};
std::vector<int> f(n + 1); 

auto add = [&](int i) {
    cur_total += f[a[i]]++;
};

auto rem = [&](int i) {
    cur_total -= --f[a[i]];
};

auto cost = [&](int l, int r) -> long long {
    while (l < curl) add(--curl);
    while (curr < r) add(++curr);

    while (curl < l) rem(curl++);
    while (r < curr) rem(curr--);
    return cur_total;
};

std::vector<long long> dp1(n), dp2(n);

auto ldp = [&](auto& dp, int l, int r, int optl, int optr) -> void {
    if (r < l) return;

    int mid {(l + r) / 2 };

    std::pair<long long, int> opt_medio{INF, -1};
    
    for (int i{optl}; i <= std::min(mid, optr); ++i) {
        long long my_cost {cost(i, mid)};
        if (i) my_cost += dp1[i - 1];

        if (opt_medio.first > my_cost) {
            opt_medio = {my_cost, i};
        }
    }
    dp2[mid] = opt_medio.first;
    auto i {opt_medio.second};
    
    dp(dp, l, mid - 1, optl, i);
    dp(dp, mid + 1, r, i, optr);
};

for (int i{}; i < n; ++i) {
    dp1[i] = cost(0, i);
}

for (int j{}; j < k - 1; ++j) {
    ldp(ldp, 0, n - 1, 0, n - 1);
    std::swap(dp1, dp2);
}
