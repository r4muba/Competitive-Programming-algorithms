struct Node {
	long long x{}, sum{};
	int lazy{};
	int value{Random::rd()};
	Node* left{};
	Node* right{};
	int sz{1};
};

void push(Node* node) {
	if (!node || !node->lazy) return;

	if (node->left) node->left->lazy ^= 1;
	if (node->right) node->right->lazy ^= 1;
	std::swap(node->left, node->right);
	node->lazy = 0;
}
 
 
int size(Node* node) {
	if (node) return node->sz;
	return 0;
}
 
void update(Node* node) {
	node->sz = (1 + size(node->left) + size(node->right));
	auto& sum {node->sum};
	sum = node->x;
	if (node->left) sum += node->left->sum;
	if (node->right) sum += node->right->sum;
}
 
auto merge(Node* a, Node* b) {
	if (!a) return b;
	if (!b) return a;
	
	push(a); push(b);
	if ((a->value) > (b->value)) {
		a->right = merge(a->right, b);
		update(a);
		return a;
	} else {
		b->left = merge(a, b->left);
		update(b);
		return b;
	}
}

std::pair<Node*, Node*> cut(Node* node, int k) {
	if (!node) return {nullptr, nullptr};
	push(node);
	
	if (size(node->left) + 1 <= k) {
		auto [my_child, other] {cut(node->right, k - size(node->left) - 1)};
		node->right = my_child;
		update(node);
		return {node, other};
	} else {
		auto [other, my_child] {cut(node->left, k)};
		node->left = my_child;
		update(node);
		return {other, node};
	}
}

/*
int l{}, r{};

auto [u, v] {cut(tree, r)};
auto [x, y] {cut(u, l - 1)};

// [    u  	 ] [ v ]
// [ x ] [ y ] [ v ]
//		   ^

tree = merge(x, (merge(y, v)));
*/
