// #pragma once
#include <iostream>

using namespace std;

enum RBTColor { Black, Red };

template<class KeyType, class T>
struct  RBTNode
{
	KeyType key;
	T value;
	RBTColor color;
	RBTNode<KeyType, T>* left;
	RBTNode<KeyType, T>* right;
	RBTNode<KeyType, T>* parent;
	RBTNode(KeyType _key, T _value, RBTColor _color, RBTNode* _parent, RBTNode* _left, RBTNode* _right) :
		key(_key), value(_value), color(_color), parent(_parent), left(_left), right(_right) { };
};

template<class KeyType, class T>
class  RBTree
{
private:
	RBTNode<KeyType, T>* root;

private:

	void left_turn(RBTNode<KeyType, T>*& root, RBTNode<KeyType, T>* node) {
		RBTNode<KeyType, T>* pivot = node->right;
		node->right = pivot->left;
		if (pivot->left)
			pivot->left->parent = node;

		pivot->parent = node->parent;
		if (!node->parent)
			root = pivot;
		else {
			if (node == node->parent->left)
				node->parent->left = pivot;
			else
				node->parent->right = pivot;
		}
		pivot->left = node;
		node->parent = pivot;
	}
	void right_turn(RBTNode<KeyType, T>*& root, RBTNode<KeyType, T>* node) {
		RBTNode<KeyType, T>* pivot = node->left;
		node->left = pivot->right;
		if (pivot->right)
			pivot->right->parent = node;

		pivot->parent = node->parent;
		if (!node->parent)
			root = pivot;
		else
			if (node == node->parent->right)
				node->parent->right = pivot;
			else
				node->parent->left = pivot;
		pivot->right = node;
		node->parent = pivot;
	}

	void insert(RBTNode<KeyType, T>*& root, RBTNode<KeyType, T>* node) {
		RBTNode<KeyType, T>* tmp = root;
		RBTNode<KeyType, T>* tmp_parent = NULL;

		while (tmp) {
			tmp_parent = tmp;
			if (node->key > tmp->key)
				tmp = tmp->right;
			else
				tmp = tmp->left;
		}
		node->parent = tmp_parent;
		if (tmp_parent)
			if (node->key > tmp_parent->key)
				tmp_parent->right = node;
			else
				tmp_parent->left = node;
		else
			root = node;
		node->color = Red;
		fix_insertion(root, node);
	}
	void fix_insertion(RBTNode<KeyType, T>*& root, RBTNode<KeyType, T>* node) {
		RBTNode<KeyType, T>* parent;
		parent = node->parent;
		while (node != RBTree::root && parent->color == Red) {
			RBTNode<KeyType, T>* grandparent = parent->parent;

			if (grandparent->left == parent) {
				// отец - левый ребЄнок
				RBTNode<KeyType, T>* uncle = grandparent->right;

				if (uncle && uncle->color == Red) {
					// случай, когда есть д€д€
					parent->color = Black;
					uncle->color = Black;
					grandparent->color = Red;
					node = grandparent;
					parent = node->parent;
				}
				else {
					// случай, когда нет д€ди
					if (parent->right == node) {
						left_turn(root, parent);
						swap(node, parent);
					}
					right_turn(root, grandparent);
					grandparent->color = Red;
					parent->color = Black;
					break;
				}
			}
			else {
				// отец - правый ребенок
				RBTNode<KeyType, T>* uncle = grandparent->left;

				if (uncle && uncle->color == Red) {
					// случай, когда есть д€д€
					grandparent->color = Red;
					parent->color = Black;
					uncle->color = Black;
					node = grandparent;
					parent = node->parent;
				}
				else {
					// случай, когда нет д€ди
					if (parent->left == node) {
						right_turn(root, parent);
						swap(parent, node);
					}
					left_turn(root, grandparent);
					parent->color = Black;
					grandparent->color = Red;
					break;
				}
			}
		}
		root->color = Black; // восстанавливаем корень
	}

	void remove(RBTNode<KeyType, T>*& root, RBTNode<KeyType, T>* node) {
		RBTNode<KeyType, T>* child, *parent;
		RBTColor color;

		//”зел слева и справа от удаленного узла не €вл€етс€ конечным
		if (node->left && node->right) {
			RBTNode<KeyType, T>* replace = node;

			// »щем узел-преемник
			replace = node->right;
			while (replace->left)
				replace = replace->left;

			// ”дал€емый узел не €вл€етс€ корневым узлом
			if (node->parent)
				if (node->parent->left == node)
					node->parent->left = replace;
				else
					node->parent->right = replace;
			// ”дал€емый узел €вл€етс€ корневым узлом
			else
				root = replace;

			child = replace->right;
			parent = replace->parent;
			color = replace->color;

			// ”зел замен€етс€ родительским узлом
			if (parent == node)
				parent = replace;
			else {
				// ƒочерний узел существует
				if (child)
					child->parent = parent;
				parent->left = child;
				replace->right = node->right;
				node->right->parent = replace;
			}
			replace->parent = node->parent;
			replace->color = node->color;
			replace->left = node->left;
			node->left->parent = replace;
			// ѕри удалении черной вершины могла быть нарушена балансировка
			if (color == Black)
				fix_deleting(root, child, parent);
			delete node;
			return;
		}
		//  огда удал€емый узел имеет только левый/правый
		if (node->left)
			child = node->left;
		else
			child = node->right;

		parent = node->parent;
		color = node->color;
		if (child)
			child->parent = parent;

		// ”дал€емый узел не €вл€етс€ корневым узлом
		if (parent)
			if (node == parent->left)
				parent->left = child;
			else
				parent->right = child;

		// ”дал€емый узел €вл€етс€ корневым узлом
		else
			RBTree::root = child;

		// ѕри удалении черной вершины могла быть нарушена балансировка
		if (color == Black)
			fix_deleting(root, child, parent);
		delete node;
	}
	void fix_deleting(RBTNode<KeyType, T>*& root, RBTNode<KeyType, T>* node, RBTNode<KeyType, T>* parent) {
		RBTNode<KeyType, T>* brother;
		while ((!node) || node->color == Black && node != RBTree::root) {
			if (parent->left == node) {
				brother = parent->right;
				if (brother->color == Red) {
					brother->color = Black;
					parent->color = Red;
					left_turn(root, parent);
					brother = parent->right;
				}
				else {
					if (!(brother->right) || brother->right->color == Black) {
						brother->left->color = Black;
						brother->color = Red;
						right_turn(root, brother);
						brother = parent->right;
					}
					brother->color = parent->color;
					parent->color = Black;
					brother->right->color = Black;
					left_turn(root, parent);
					node = root;
					break;
				}
			}
			else {
				brother = parent->left;
				if (brother->color == Red) {
					brother->color = Black;
					parent->color = Red;
					right_turn(root, parent);
					brother = parent->left;
				}
				if ((!brother->left || brother->left->color == Black) && (!brother->right || brother->right->color == Black)) {
					brother->color = Red;
					node = parent;
					parent = node->parent;
				}
				else {
					if (!(brother->left) || brother->left->color == Black) {
						brother->right->color = Black;
						brother->color = Red;
						left_turn(root, brother);
						brother = parent->left;
					}
					brother->color = parent->color;
					parent->color = Black;
					brother->left->color = Black;
					right_turn(root, parent);
					node = root;
					break;
				}
			}
		}
		if (node)
			node->color = Black;
	}

	void destroy(RBTNode<KeyType, T>*& node) {
		if (node) {
			destroy(node->left);
			destroy(node->right);
			delete node;
			node = nullptr;
		}
	}
	RBTNode<KeyType, T>* find(RBTNode<KeyType, T>* node, KeyType key) const {
		if ((!node) || node->key == key)
			return node;
		else
			return key > node->key ? find(node->right, key) : find(node->left, key);
	}

	void print(RBTNode<KeyType, T>* node) const {
		if (node) {
			if (!node->parent)
				cout << node->key << '[' << node->color << "]	--	root" << endl;
			else if (node->parent->left == node)
				cout << node->key << '[' << node->color << "]	--	" << node->parent->key << "'s " << "left child" << endl;
			else
				cout << node->key << '[' << node->color << "]	--	" << node->parent->key << "'s " << "right child" << endl;
			print(node->left);
			print(node->right);
		}
	}


public:
	RBTree() : root(nullptr) {}
	~RBTree() { destroy(root); }

	void insert(KeyType key, T value) {
		insert(root, new RBTNode<KeyType, T>(key, value, Red, NULL, NULL, NULL));
	}

	void erase(KeyType key) {
		RBTNode<KeyType, T>* deletenode = find(root, key);
		if (deletenode)
			remove(root, deletenode);
	}

	RBTNode<KeyType, T>* find(KeyType key) { return find(root, key); }

	void print() {
		if (!root)
			cout << "Empty tree" << '\n';
		else
			print(root);
	}

};
/*
// TESTING
int main() {
    RBTree<int, int> t;
    t.insert(5, 5);
    t.insert(6, 6);
    t.insert(-1, -1);
    RBTNode<int, int>* f = t.find(5);
    cout << f->value << '\n';
    t.print();

}
*/
