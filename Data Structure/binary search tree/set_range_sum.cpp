﻿#include <cstdio>
#include <iostream>
using namespace std;

// Splay tree implementation

// Vertex of a splay tree
struct Vertex {
	long long  key;
	// Sum of all the keys in the subtree - remember to update
	// it after each operation that changes the tree.
	long long sum;
	Vertex* left;
	Vertex* right;
	Vertex* parent;

	Vertex(long long key, long long sum, Vertex* left, Vertex* right, Vertex* parent)
		: key(key), sum(sum), left(left), right(right), parent(parent) {}
};

void update(Vertex* v) {
	if (v == NULL) return;
	v->sum = v->key + (v->left != NULL ? v->left->sum : 0ll) + (v->right != NULL ? v->right->sum : 0ll);
	if (v->left != NULL) {
		v->left->parent = v;
	}
	if (v->right != NULL) {
		v->right->parent = v;
	}
}

void small_rotation(Vertex* v) { // v的前提是有parent
	Vertex* parent = v->parent;
	if (parent == NULL) {
		return;
	}
	Vertex* grandparent = v->parent->parent;//最后要挂在grandparent 上
	if (parent->left == v) {  //从左往右移动
		Vertex* m = v->right;
		v->right = parent;
		parent->left = m;
	}
	else { //从右往左移动
		Vertex* m = v->left;
		v->left = parent;
		parent->right = m;
	}
	update(parent);
	update(v);
	v->parent = grandparent; //挂在grandparent 上
	if (grandparent != NULL) {
		if (grandparent->left == parent) {//给grandparent assign child
			grandparent->left = v;
		}
		else {
			grandparent->right = v;
		}
	}
}

void big_rotation(Vertex* v) {
	if (v->parent->left == v && v->parent->parent->left == v->parent) {
		// Zig-zig
		small_rotation(v->parent);
		small_rotation(v);
	}
	else if (v->parent->right == v && v->parent->parent->right == v->parent) {
		// Zig-zig
		small_rotation(v->parent);
		small_rotation(v);
	}
	else {
		// Zig-zag
		small_rotation(v);
		small_rotation(v);
	}
}

// Makes splay of the given vertex and makes
// it the new root.
void splay(Vertex*& root, Vertex* v) {
	if (v == NULL) return;
	while (v->parent != NULL) {
		if (v->parent->parent == NULL) {
			small_rotation(v);
			break;
		}
		big_rotation(v);
	}
	root = v;
}

// Searches for the given key in the tree with the given root
// and calls splay for the deepest visited node after that.
// If found, returns a pointer to the node with the given key.
// Otherwise, returns a pointer to the node with the smallest
// bigger key (next value in the order).
// If the key is bigger than all keys in the tree, 
// returns NULL.
Vertex* find(Vertex*& root, long long key) { // 需要v.key 等于key的，然后把最深的root，给splay
	Vertex* v = root;
	Vertex* last = root;
	Vertex* next = NULL;
	while (v != NULL) {
		if (v->key >= key && (next == NULL || v->key < next->key)) {//key小于等于 v->key, next->key 大于等于next->key
																	// 需要下一个比key 大的元素
			next = v;
		}
		last = v;
		if (v->key == key) {
			break;
		}
		if (v->key < key) {
			v = v->right;
		}
		else {
			v = v->left;
		}
	}
	splay(root, last);
	return next;
}

void split(Vertex* root, long long key, Vertex*& left, Vertex*& right) {
	right = find(root, key); // 把key 挂在right 上
	splay(root, right);
	if (right == NULL) {
		left = root;
		return;
	}
	left = right->left;
	right->left = NULL;
	if (left != NULL) {
		left->parent = NULL;
	}
	update(left);
	update(right);
}

Vertex* merge(Vertex* left, Vertex* right) {
	if (left == NULL) return right;
	if (right == NULL) return left;
	Vertex* min_right = right;
	while (min_right->left != NULL) {
		min_right = min_right->left;
	}
	splay(right, min_right);
	right->left = left;
	update(right);
	return right;
}

// Code that uses splay tree to solve the problem

Vertex* root = NULL;

void insert(long long x) {
	Vertex* left = NULL;
	Vertex* right = NULL;
	Vertex* new_vertex = NULL;
	split(root, x, left, right);
	if (right == NULL || right->key != x) { // 如果最右边是空的，或者右边的tree 不是x，需要新建一个root，然后和left 先merge，然后跟right merge
		new_vertex = new Vertex(x, x, NULL, NULL, NULL);
	}
	root = merge(merge(left, new_vertex), right);
}

void erase(long long x) {
	//Vertex* left = NULL;
	//Vertex* right = NULL;
	//Vertex* new_vertex = NULL;
	//split(root, x, left, right);
	//if (right!= NULL && right->key == x) {
	//	root = merge(left, right->right);
	//}
	//else {
	//	root = merge(left, right);
	//}
	Vertex* left = NULL;
	Vertex* middle = NULL;
	Vertex* right = NULL;
	split(root, x, left, middle);
	split(middle, x + 1, middle, right);
	root = merge(left, right);
	// Implement erase yourself
	//splay()
	//if (root == NULL) return;
	//Vertex* next_x = find(root, x + 1);
	//if (next_x == NULL) {
	//	if (root->key == x) {
	//		root = root->left;
	//		if (root != NULL) root->parent = NULL;
	//		update(root);
	//	}
	//	return;
	//}
	//else {
	//	Vertex* xx = find(root, x);
	//	if (xx->key != x) return;
	//	Vertex* left = root->left;
	//	Vertex* right = root->right;
	//	if(right!=NULL) right->left = left;
	//	if (left != NULL) left->parent = right;
	//	root = right;
	//	if (root != NULL) root->parent = NULL;
	//	update(root);
	//}

}

bool find(long long x) {
	// Implement find yourself
	Vertex* new_vertex = find(root, x);
	if (new_vertex != NULL && new_vertex->key == x) return true;

	return false;
}

long long sum(long long  from, long long to) {
	Vertex* left = NULL;
	Vertex* middle = NULL;
	Vertex* right = NULL;
	split(root, from, left, middle);
	split(middle, to + 1, middle, right);
	long long ans = 0;
	// Complete the implementation of sum
	if (middle != NULL)
		ans += middle->sum;
	//if (right != NULL && right->key == to) ans += right->key;
	//if (right != NULL) ans += right->sum;
	root = merge(merge(left, middle), right);
	return ans;
}

const long long MODULO = 1000000001;

void loop(Vertex* root) {
	if (root == NULL) return;
	cout << root->key << "  ";
	if (root->left != NULL) loop(root->left);
	if (root->right != NULL) loop(root->right);
}


int main() {
	long long n;
	//scanf("%d", &n);
	cin >> n;
	long long  last_sum_result = 0;
	for (long long i = 0; i < n; i++) {
		char type;
		cin >> type;
		//scanf("%s", buffer);
		//char type = buffer[0];
		switch (type) {
		case '+': {
			long long x;
			cin >> x;
			//scanf("%d", &x);
			insert((x + last_sum_result) % MODULO);
		} break;
		case '-': {
			long long x;
			//scanf("%d", &x);
			cin >> x;
			erase((x + last_sum_result) % MODULO);
		} break;
		case '?': {
			long long x;
			//scanf("%d", &x);
			cin >> x;
			cout << (find((x + last_sum_result) % MODULO) ? "Found" : "Not found") << endl;
			//printf(find((x + last_sum_result) % MODULO) ? "Found\n" : "Not found\n");
		} break;
		case 's': {
			long long l, r;
			//scanf("%d %d", &l, &r);
			cin >> l >> r;
			long long res = sum((l + last_sum_result) % MODULO, (r + last_sum_result) % MODULO);
			//printf("%lld\n", res);
			cout << res << endl;
			last_sum_result = (long long)(res % MODULO);
		}
		}
		/*loop(root);
		cout << endl<<endl;*/
	}
	return 0;
}
