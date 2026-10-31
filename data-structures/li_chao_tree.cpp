class LiChaoTree {
    struct Line {
        long long m{}, b{INF};
        long long operator()(long long x) { return (m * x) + b; }
    };
    struct Node {
        Line line{};
        Node* left{};
        Node* right{};
    };
public:
    LiChaoTree () : root {new Node{}} {}
    void add (long long m, long long b) {
        insert(root, 0, n, Line{m, b});
    }
    long long operator()(long long x) { 
        return query(root, 0, n, x); 
    }
    ~LiChaoTree() { 
        auto ldfs = [&](auto& dfs, Node* node) -> void {
            if (!node) return;
            dfs(dfs, node->left);
            dfs(dfs, node->right);
            delete node;
        };
        ldfs(ldfs, root);
    }
private:
    const int n = 1e9;
    Node* root{};
 
    int mid(int l, int r) { return l + ((r - l) / 2); }
 
    Node* insert(Node* node, int l, int r, Line cur_line) {
        if (!node) return new Node {cur_line};
 
        auto x {mid(l, r)};
        if (l == r) {
            if (node->line(x) > cur_line(x)) {
                std::swap(node->line, cur_line);
            }
            return node;
        }
        if (node->line.m > cur_line.m) {
            std::swap(node->line, cur_line);
        }
        if (node->line(x) <= cur_line(x)) {
            node->left = insert(node->left, l, mid(l, r), cur_line);
        } else {
            std::swap(node->line, cur_line);
            node->right = insert(node->right, mid(l, r) + 1, r, cur_line);
        }
        return node;
    }
 
    long long query(Node* node, int l, int r, long long x) {
        if (!node) return INF;
        if (l == r) return node->line(x);
 
        return std::min ( 
            node->line(x),
            (x <= mid(l, r)) ? 
                query(node->left, l, mid(l, r), x) 
                : query(node->right, mid(l, r) + 1, r, x)
        );
    }
};
