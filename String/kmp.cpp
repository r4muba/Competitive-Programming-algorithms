#include <bits/stdc++.h>

auto kmp = [](std::string_view s, std::string_view p) {
    int n{std::ssize(s)}, m{std::ssize(p)};
    std::vector<int> b(m + 1);
    int i{}, j{-1};
    b[0] = -1;
    while (i < m) {
        while ((j >= 0) && (p[i] != p[j])) j = b[j];
        ++i; ++j;
        b[i] = j;
    }

    std::vector<int> ans;
    i = 0; j = 0;
    while (i < n) {
        while ((j >= 0) && (s[i] != p[j])) j = b[j];
        ++i; ++j;
        if (j == m) {
            ans.push_back(i - m);
            j = b[j];
        }
    }
    return ans;
};
