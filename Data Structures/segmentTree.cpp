#include <bits/stdc++.h>

class SegmentTree {
    using T = int;
public:
    SegmentTree (const std::vector<T>& a) 
        : n{std::ssize(a)}, tree(n * 4, kdefault), lazy(n * 4, klazy) {
            build(0, 0, n - 1, a);
        }
    void increment (int l, int r, T delta);
    T query        (int l, int r);
private:
    int n{};
    const T kdefault{}, klazy{};
    std::vector<T> tree{}, lazy{};

    T operation (T a, T b) { return std::max(a, b); }
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
        tree[node] = operation(tree[left(node)], tree[right(node)]);
    }
    void increment(int node, int l, int r, int leftQuery, int rightQuery, T delta) {
        propagate(node, l, r);
        if (r < leftQuery || rightQuery < l) return;
        if (leftQuery <= l && r <= rightQuery) {
            lazy[node] += delta;
            propagate(node, l, r);
            return;
        }
        increment(left(node), l, mid(l, r),      leftQuery, rightQuery, delta);
        increment(right(node), mid(l, r) + 1, r, leftQuery, rightQuery, delta);
        tree[node] = operation(tree[left(node)], tree[right(node)]);
    }
    T query(int node, int l, int r, int leftQuery, int rightQuery) {
        if (r < leftQuery || rightQuery < l) return kdefault;
        propagate(node, l, r);
        if (leftQuery <= l && r <= rightQuery) return tree[node];
        return operation ( 
            query(left(node), l, mid(l, r), leftQuery, rightQuery),
            query(right(node), mid(l, r) + 1, r, leftQuery, rightQuery)
        );
    }
    void propagate (int node, int l, int r) { 
        if (lazy[node] == klazy) return;
        tree[node] += lazy[node];
        if (l != r) {
            lazy[left(node)]  += lazy[node];
            lazy[right(node)] += lazy[node];
        }
        lazy[node] = klazy;
    }
};
