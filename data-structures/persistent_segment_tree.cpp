class SegmentTree {
	struct Node {
		long long sum{};
		Node* left{};
	    Node* right{};
	};
public:
	SegmentTree (const std::vector<int>& a) : n{static_cast<int>(a.size())} {
		tree.push_back(build(0, n - 1, a));
	}
	long long query(int l, int r, int k) {
		return query(tree[k - 1], 0, n - 1, l - 1, r - 1);
	}
	void copy(int k) {
		k--;
		tree.push_back(new Node{*tree[k]});
	}
	void update (int i, long long value, int k) {
		update(tree[k - 1], 0, n - 1, i - 1, value);
	}
private:
	int n{};
	std::vector<Node*> tree{};
	int mid(int a, int b) { return (a + b) / 2; }

	Node* build(int l, int r, const std::vector<int>& a) {
		if (l == r) {
			return new Node {a[l]};
		}
		auto* node = new Node{};
		node->left = build(l, mid(l, r), a);
		node->right = build(mid(l, r) + 1, r, a);
		
		node->sum = node->left->sum + node->right->sum;
		return node;
	}
	long long query (Node* node, int l, int r, int leftq, int rightq) {
		if (r < leftq || rightq < l) return 0;
		if (leftq <= l && r <= rightq) {
			return node->sum;
		}
		return query(node->left, l, mid(l, r), leftq, rightq)
			 + query(node->right, mid(l, r) + 1, r, leftq, rightq);
	}
	void update(Node* node, int l, int r, int i, long long  x) {
		if (l == r) {
			node->sum = x;
			return;
		}
		if (i <= mid(l, r)) {
			auto* left {new Node{*node->left}};
			node->left = left;
			update(node->left, l, mid(l, r), i, x);
		} else {
			auto* right {new Node{*node->right}};
			node->right = right;
			update(node->right, mid(l, r) + 1, r, i, x);
		}
		node->sum = node->left->sum + node->right->sum;
	}
};
