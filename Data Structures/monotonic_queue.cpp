// Monotonic Stack
std::vector<int> stk{-1};
stk.reserve(n + 1);

for (int i{}; i < n; ++i) {
    while (stk.back() != -1 && a[stk.back()] <= a[i]) stk.pop_back();
    // process
    stk.push_back(i);
}

// Monotonic deque
int m { /* window size */}; 
std::deque<int> dq{};

for (int i{}; i < n; ++i) {
    while (!dq.empty() && a[dq.back()] <= a[i]) dq.pop_back();
    dq.push_back(i);
    if (dq.front() <= i - m) dq.pop_front();

    if (i > m) {
        int max_value {a[dq.front()]};
    }
}
