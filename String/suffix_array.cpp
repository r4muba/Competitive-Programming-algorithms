// Push back a sentinel {static_cast<char>(1)} to the string before SuffixArray
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

// Longest-Repeated-Substring O(N)
// Returns {length, start_index} of one occurrence (lexicographically smallest).
std::pair<int,int> lrs (SuffixArray& sa) {
    const auto& lcp {sa.get_lcp()};
    const auto& suffix {sa.get_suffix()};

    std::pair<int,int> ans{-1, -1};
    for (int i{}; i < sa.size(); ++i) {
        if (lcp[i] > ans.first) {
            ans = {lcp[i], suffix[i]};
        }
    }
    return ans;
}

// Longest-Repeated-Substring-Occurrence-Count O(N)
// Finds the longest substring that repeats, and how many times it occurs.
// Returns {length, occurrence_count} of that substring.
// If no substring repeats, returns {0, 1} (every substring occurs once).
std::pair<int,int> lrso(SuffixArray& sa) {
    const auto& lcp{sa.get_lcp()};
    int m{std::ssize(lcp)};

    int len{};
    for (int x : lcp) len = std::max(len, x);
    if (len == 0) return {0, 1}; 

    int ans{1}, run{};
    for (int i{}; i < m; ++i) {
        if (lcp[i] >= len) {
            ++run;
            ans = std::max(ans, run + 1);
        } else {
            run = 0;
        }
    }
    return {len, ans};
}

// Count-Distinct-Substrings O(N)
long long cds (SuffixArray& sa) {
    long long n{sa.size() - 1}, ans {(n * (n + 1)) / 2};
    for (auto u : sa.get_lcp()) ans -= u;
    return ans;
}

// Count-Distinct-Substrings-Repetead-At-Least-K O(N)
long long cdrk (SuffixArray& sa, int k) {
    const auto& lcp{sa.get_lcp()};
    int n {std::ssize(lcp)};
    if (k < 1 || n < k) return 0;
    if (k == 1) {
        return cds(sa);
    }

    std::deque<int> dq{};
    long long ans{};
    int x{};

    for (int i{1}; i < n; ++i) {
        while (!dq.empty() && lcp[dq.back()] >= lcp[i]) dq.pop_back();
        dq.push_back(i);
        if (dq.front() <= i - (k - 1)) dq.pop_front();
        int l{i - (k - 2)};
        if (l < 1) continue;

        ans += std::max(0, lcp[dq.front()] - x);
        x = lcp[dq.front()];
    }
    return ans;
}

std::string kth_distinct_substring(SuffixArray& sa, const std::string& s, long long k) {
    const auto& suf = sa.get_suffix();
    const auto& lcp = sa.get_lcp();
    const int n = std::ssize(s) - 1;   

    for (int i{}; i < std::ssize(suf); ++i) {
        long long prev = (i == 0) ? 0 : lcp[i];
        long long m = (n - suf[i]) - prev;
        if (m <= 0) continue; 
        if (k <= m) return s.substr(suf[i], prev + k);
        k -= m;
    }
    return "";
}

// Longest-Repetead-Substring-At-Least-K O(N)
// Returns {length, start_index} of one valid occurrence — specifically
// the occurrence with the largest/smallest starting index among all substrings
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

// Longest-Repetead-Substring-By-Lenght O(N)
// Returns a vector of size n+1; index L (1..n) holds max occurrences of
// length-L substrings. 
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

// LCP(i, j): longest common prefix of suffixes starting at i, j.
// i == j ---> n - i (whole suffix, no RMQ).
// i != j ---> let lo,hi = minmax(rank[i], rank[j]); RMQ(lcp[lo+1..hi]).

// Helper function to concatend strings for lcks
// preprocess the suffix_belongs
// Return string, ready to SuffixArray
int offset {105};
std::vector<int> suffix_belongs{};
std::vector<int> concatenated_strings (std::vector<std::string>& s) {
    std::vector<int> a{};
    suffix_belongs.clear();
    int maxn{};
    for (auto& u : s) maxn += std::ssize(u) + 1;
    a.reserve(maxn);
    suffix_belongs.reserve(maxn);

    for (int i{}; i < std::ssize(s); ++i) {
        for (auto c : s[i]) {
            a.push_back(c + offset);
            suffix_belongs.push_back(i);
        }
        a.push_back(static_cast<char>(1 + i));
        suffix_belongs.push_back(-1);
    }
    return a;
}

// Longest-Common-Substring-At-Least-K-Different Strings
// Finding the longest common substring that appears in at least K 
// different strings out of a given collection of strings.
// Return the max {length, vector<>{index}}
// Most of the time should work with integers and offset them to a wider range
// 1->(k + 1) sentinel char, A = (k + 1) -> Z = (k + 1 + 'A')
std::pair<int, std::vector<int>> lcsk (SuffixArray& sa, int k) {
    int n {sa.size()};
    const auto& lcp {sa.get_lcp()};
    const auto& suffix {sa.get_suffix()};

    std::unordered_map<int, int> f{};
    int distinct{};
    std::deque<int> dq{};

   
    auto addSuffix = [&](int i) {
        if (i < 0) return;
        if (f[i]++ == 0) ++distinct;
    };
    auto rmSuffix = [&](int i) {
        if (i < 0) return;
        if (--f[i] == 0) --distinct;
    };

     std::pair<int, std::vector<int>> ans{};

    int l{};
    for (int i{}; i < n; ++i) {
        addSuffix(suffix_belongs[suffix[i]]);
        while ((!dq.empty()) && lcp[dq.back()] >= lcp[i]) dq.pop_back();
        if (i > l) dq.push_back(i);

        while (distinct >= k) {
            if (!dq.empty()) {
                int len {lcp[dq.front()]};
                if (len > ans.first) {
                    ans = {len, std::vector{suffix[l]}};
                } else if (len == ans.first && len > 0) {
                    ans.second.push_back(suffix[l]);
                }
            }
            rmSuffix(suffix_belongs[suffix[l]]);
            if ((!dq.empty()) && dq.front() == l + 1) dq.pop_front();
            ++l;
        }
    }
    return ans;
}
