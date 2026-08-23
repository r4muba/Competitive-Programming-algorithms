#include<bits/stdc++.h>
/*
	TODO: Add info about the functions
		  Test the lrsso, distinctSubstrings, kthSubstring
		  Add the rmq () of lcp with sparse_table / segtree
		  add Longest Common Substring of k strings
*/
class SuffixArray {
public:
    SuffixArray (std::string_view a)
        : s{a}, n{static_cast<int>(std::ssize(a))},
        suffix(n), rank(n), lcp(n) {
            build();
            buildlcp();
        }
    int size() { return n; }
    const auto& get_lcp() { return lcp; }
    const auto& get_suffix() { return suffix; }
private:
    std::string_view s{};
    int n{};
    std::vector<int> suffix{}, rank{}, lcp{};
    
    void csort(int k) {
        int m{std::max(n, 300)};
        std::vector<int> f(m);
        for (int i{}; i < n; ++i) {
            ++f[(i + k < n) ? rank[i + k] : 0];
        }
        for (int i{1}; i < m; ++i) f[i] += f[i - 1];
        std::vector<int> b{suffix.rbegin(), suffix.rend()};
        for (auto u : b) {
            int r{(u + k < n) ? rank[u + k] : 0};
            suffix[--f[r]] = u;
        }
    }
    void build() {
        std::iota(suffix.begin(), suffix.end(), 0);
        for (int i{}; i < n; ++i) rank[i] = s[i];

        for (int k{1}; k < n; k *= 2) {
            csort(k);
            csort(0);
            std::vector<int> tr(n);
            int r{};
            tr[suffix[0]] = r;
            for (int i{1}; i < n; ++i) {
                int cur {suffix[i]};
                int prev {suffix[i - 1]};
                tr[cur] = (
                    (rank[cur] == rank[prev])
                &&  (rank[cur + k] == rank[prev + k])
                ) ? r : ++r;
            }
            std::swap(rank, tr);
            if (rank[suffix[n - 1]] == n - 1) break;
        }
    }
    void buildlcp() {
        std::vector<int> phi(n), plcp(n);
        phi[suffix[0]] = -1;
        for (int i{1}; i < n; ++i) phi[suffix[i]] = suffix[i - 1];
        for (int i{}, len{}; i < n; ++i) {
            if (phi[i] == -1) {
                plcp[i] = 0;
                continue;
            }
            while ((i + len < n) && (phi[i] + len < n)
                && (s[i + len] == s[phi[i] + len]) ) ++len;
            plcp[i] = len;
            len = std::max(len - 1, 0);
        }
        for (int i{}; i < n; ++i) {
            lcp[i] = plcp[suffix[i]];
        }
    }
};

int lrss (SuffixArray& a) {
	const auto& lcp {a.get_lcp()};
	return *std::max_element(lcp.begin(), lcp.end());
}

std::pair<int,int> lrsso(SuffixArray& sa) {
    const auto& lcp{sa.get_lcp()};
    int m{std::ssize(lcp)};

    int L{};
    for (int x : lcp) L = std::max(L, x);
    if (L == 0) return {0, 1}; 

    int best_occurrences{1}, run{};
    for (int i{}; i < m; ++i) {
        if (lcp[i] >= L) {
            ++run;
            best_occurrences = std::max(best_occurrences, run + 1);
        } else {
            run = 0;
        }
    }
    return {L, best_occurrences};
}

long long distinctSubstrings(SuffixArray& sa) {
	long long n{sa.size() - 1};
	long long ans{(n * (n + 1)) / 2};
	for (int x : sa.get_lcp()) ans -= x;
	return ans;
}

std::string kthSubstring(long long k, SuffixArray& sa, const std::string& s) {
    const auto& suf = sa.get_suffix();
    const auto& lcp = sa.get_lcp();
    for (int i{}; i < std::ssize(suf); ++i) {
        long long newCount = (std::ssize(s) - suf[i]) - (i == 0 ? 0 : lcp[i]);
        if (k <= newCount) return s.substr(suf[i], lcp[i] + k);
        k -= newCount;
    }
    return ""; 
}

std::pair<int,int> lrsk(SuffixArray& a, int k) {
    int n{a.size()};
    int m {k - 1};
    if (m > n - 1) return {0, 0};
    if (k <= 1) return {n - 1, 0};

    const auto& lcp {a.get_lcp()};
    const auto& suffix {a.get_suffix()};
    std::deque<int> dq, dqpos{};
    
    int best{}, idx{-1};
    for (int i{}; i < n; ++i) {
        while (!dq.empty() && lcp[dq.back()] >= lcp[i]) dq.pop_back();
        dq.push_back(i);
        if (dq.front() <= i - m) dq.pop_front();

        while (!dqpos.empty() && suffix[dqpos.back()] <= suffix[i]) dqpos.pop_back();
        dqpos.push_back(i);
        if (dqpos.front() < i - m) dqpos.pop_front();

        if (i >= m) {
            int maxpos {suffix[dqpos.front()]};
            if (best < lcp[dq.front()]) {
                best = lcp[dq.front()];
                idx = maxpos;
            } else if (best == lcp[dq.front()] && best > 0) {
                idx = std::max(idx, maxpos);
            }
        }
    }
    if (best == 0) return {};
    return {best, idx};
}

std::vector<int> lrbl(SuffixArray& sa) {
    int n {sa.size()};
    const auto& lcp {sa.get_lcp()};
    std::vector<int> left(n), right(n), stk{-1};
    stk.reserve(n + 1);

    for (int i{}; i < n; ++i) {
        while (stk.back() != -1 && lcp[stk.back()] >= lcp[i]) stk.pop_back();
        left[i] = stk.back();
        stk.push_back(i);
    }

    stk.clear();
    stk.push_back(n);
    for (int i{n - 1}; i >= 0; --i) {
        while (stk.back() != n && lcp[stk.back()] >= lcp[i]) stk.pop_back();
        right[i] = stk.back();
        stk.push_back(i);
    }

    std::vector<int> ans(n + 1, 1);
    for (int i{}; i < n; ++i) {
        int width = right[i] - left[i] - 1;
        ans[lcp[i]] = std::max(ans[lcp[i]], width + 1);
    }

    for (int i{n - 1}; i >= 1; --i) {
        ans[i] = std::max(ans[i], ans[i + 1]);
    }
    return ans;
}
