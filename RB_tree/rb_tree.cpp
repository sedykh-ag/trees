// #pragma once
#include <iostream>

using namespace std;

enum RBTColor { Black, Red };

template<class KeyType>
struct  RBTNode
{
	KeyType key;
	RBTColor color;
	RBTNode<KeyType>* left;
	RBTNode<KeyType>* right;
	RBTNode<KeyType>* parent;
	RBTNode(KeyType _key, RBTColor _color, RBTNode* _parent, RBTNode* _left, RBTNode* _right) :
		key(_key), color(_color), parent(_parent), left(_left), right(_right) { };
};

template<class KeyType>
class  RBTree
{
private:
	RBTNode<KeyType>* root;

private:

	void left_turn(RBTNode<KeyType>*& root, RBTNode<KeyType>* node) {
		RBTNode<KeyType>* pivot = node->right;
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
	void right_turn(RBTNode<KeyType>*& root, RBTNode<KeyType>* node) {
		RBTNode<KeyType>* pivot = node->left;
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

	void insert(RBTNode<KeyType>*& root, RBTNode<KeyType>* node) {
		RBTNode<KeyType>* tmp = root;
		RBTNode<KeyType>* tmp_parent = NULL;

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
	void fix_insertion(RBTNode<KeyType>*& root, RBTNode<KeyType>* node) {
		RBTNode<KeyType>* parent;
		parent = node->parent;
		while (node != RBTree::root && parent->color == Red) {
			RBTNode<KeyType>* grandparent = parent->parent;

			if (grandparent->left == parent) {
				// ���� - ����� ������
				RBTNode<KeyType>* uncle = grandparent->right;

				if (uncle && uncle->color == Red) {
					// ������, ����� ���� ����
					parent->color = Black;
					uncle->color = Black;
					grandparent->color = Red;
					node = grandparent;
					parent = node->parent;
				}
				else {
					// ������, ����� ��� ����
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
				// ���� - ������ �������
				RBTNode<KeyType>* uncle = grandparent->left;

				if (uncle && uncle->color == Red) {
					// ������, ����� ���� ����
					grandparent->color = Red;
					parent->color = Black;
					uncle->color = Black;
					node = grandparent;
					parent = node->parent;
				}
				else {
					// ������, ����� ��� ����
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
		root->color = Black; // ��������������� ������
	}

	void remove(RBTNode<KeyType>*& root, RBTNode<KeyType>* node) {
		RBTNode<KeyType>* child, *parent;
		RBTColor color;

		//���� ����� � ������ �� ���������� ���� �� �������� ��������
		if (node->left && node->right) {
			RBTNode<KeyType>* replace = node;

			// ���� ����-��������
			replace = node->right;
			while (replace->left)
				replace = replace->left;

			// ��������� ���� �� �������� �������� �����
			if (node->parent)
				if (node->parent->left == node)
					node->parent->left = replace;
				else
					node->parent->right = replace;
			// ��������� ���� �������� �������� �����
			else
				root = replace;

			child = replace->right;
			parent = replace->parent;
			color = replace->color;

			// ���� ���������� ������������ �����
			if (parent == node)
				parent = replace;
			else {
				// �������� ���� ����������
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
			// ��� �������� ������ ������� ����� ���� �������� ������������
			if (color == Black)
				fix_deleting(root, child, parent);
			delete node;
			return;
		}
		// ����� ��������� ���� ����� ������ �����/������
		if (node->left)
			child = node->left;
		else
			child = node->right;

		parent = node->parent;
		color = node->color;
		if (child)
			child->parent = parent;

		// ��������� ���� �� �������� �������� �����
		if (parent)
			if (node == parent->left)
				parent->left = child;
			else
				parent->right = child;

		// ��������� ���� �������� �������� �����
		else
			RBTree::root = child;

		// ��� �������� ������ ������� ����� ���� �������� ������������
		if (color == Black)
			fix_deleting(root, child, parent);
		delete node;
	}
	void fix_deleting(RBTNode<KeyType>*& root, RBTNode<KeyType>* node, RBTNode<KeyType>* parent) {
		RBTNode<KeyType>* brother;
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

	void destory(RBTNode<KeyType>*& node) {
		if (node) {
			destory(node->left);
			destory(node->right);
			delete node;
			node = nullptr;
		}
	}
	RBTNode<KeyType>* find(RBTNode<KeyType>* node, KeyType key) const {
		if ((!node) || node->key == key)
			return node;
		else
			return key > node->key ? find(node->right, key) : find(node->left, key);
	}

	void print(RBTNode<KeyType>* node) const {
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
	~RBTree() { destory(root); }

	void insert(KeyType key) {
		insert(root, new RBTNode<KeyType>(key, Red, NULL, NULL, NULL));
	}

	void remove(KeyType key) {
		RBTNode<KeyType>* deletenode = find(root, key);
		if (deletenode)
			remove(root, deletenode);
	}

	RBTNode<KeyType>* find(KeyType key) { return find(root, key); }

	void print() {
		if (!root)
			cout << "Empty tree" << '\n';
		else
			print(root);
	}

};


