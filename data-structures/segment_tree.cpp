class SegmentTree {
    using T = long long;
public:
    SegmentTree (const std::vector<T>& a) 
        : n{static_cast<int>(a.size())}, tree(n * 4, kdefault), lazy(n * 4, klazy) {
            build(0, 0, n - 1, a);
        }
    void update (int l, int r, T delta);
    T query     (int l, int r);
private:
    int n{};
    const T kdefault{}, klazy{};
    std::vector<T> tree{}, lazy{};

    T agg (T a, T b) { return std::max(a, b); }
    int left  (int node)     { return (node << 1) + 1; }
    int right (int node)     { return (node << 1) + 2; }
    int mid   (int a, int b) { return (a + b) / 2; }

    void build(int node, int l, int r, const std::vector<T>& a) {
        if (l == r) {
            tree[node] = a[l];
            return;
        }
        build(left(node), l, mid(l, r), a);
        build(right(node), mid(l , r) + 1, r, a);
        tree[node] = agg(tree[left(node)], tree[right(node)]);
    }
    void update(int node, int l, int r, int leftq, int rightq, T delta) {
        push(node, l, r);
        if (r < leftq || rightq < l) return;
        if (leftq <= l && r <= rightq) {
            lazy[node] += delta;
            push(node, l, r);
            return;
        }
        update(left(node), l, mid(l, r),      leftq, rightq, delta);
        update(right(node), mid(l, r) + 1, r, leftq, rightq, delta);
        tree[node] = agg(tree[left(node)], tree[right(node)]);
    }
    T query(int node, int l, int r, int leftq, int rightq) {
        if (r < leftq || rightq < l) return kdefault;
        push(node, l, r);
        if (leftq <= l && r <= rightq) return tree[node];
        return agg ( 
            query(left(node), l, mid(l, r),      leftq, rightq),
            query(right(node), mid(l, r) + 1, r, leftq, rightq)
        );
    }
    void push(int node, int l, int r) { 
        if (lazy[node] == klazy) return;
        tree[node] += lazy[node];
        if (l != r) {
            lazy[left(node)]  += lazy[node];
            lazy[right(node)] += lazy[node];
        }
        lazy[node] = klazy;
    }
};
