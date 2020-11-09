#ifndef AVL_TREE_H
#define AVL_TREE_H

#include "node.h"
#include<memory>
#include<iostream>
#include <initializer_list>

template <class KeyType, class ValueType>
using node_ptr_t = std::shared_ptr<node<KeyType,ValueType>>;

template <class KeyType, class ValueType>
class Avl_tree {
private:
	//variables
	node_ptr_t<KeyType, ValueType> root;

	// functions

	// const
	void print(node_ptr_t<KeyType, ValueType> p, int level) const; // recursive print ������
	unsigned int get_height(node_ptr_t<KeyType, ValueType> p) const { return p != nullptr ? p->height : 0; }	// ���������� ������ "������" ����
	int bfactor(node_ptr_t<KeyType, ValueType> p) const { return get_height(p->right) - get_height(p->left); }	// ������ ������������ ����
	node_ptr_t<KeyType, ValueType> find_min(node_ptr_t<KeyType, ValueType> p) const { return p->left ? find_min(p->left) : p; }	// ����� ������������ key � ��������� ������� ����
	node_ptr_t<KeyType, ValueType> find(node_ptr_t<KeyType, ValueType> p, const KeyType& input_key) const; // recursive ����� ���� � �������� ������

	//not const
	node_ptr_t<KeyType, ValueType> insert(const KeyType& input_key, const ValueType& input_value, node_ptr_t<KeyType, ValueType> leaf);	// recursive insert key �� ���������
	node_ptr_t<KeyType, ValueType> insert(const KeyType& input_key, node_ptr_t<KeyType, ValueType> leaf);	// recursive insert key ��� ��������
	void fixheight(node_ptr_t<KeyType, ValueType> p);	// ������ ������ ����
	node_ptr_t<KeyType, ValueType> rotateright (node_ptr_t<KeyType, ValueType> p); // ������ ������� ������ p
	node_ptr_t<KeyType, ValueType> rotateleft (node_ptr_t<KeyType, ValueType> q); // ����� ������� ������ q
	node_ptr_t<KeyType, ValueType> balance(node_ptr_t<KeyType, ValueType> p); // ������������ ���� p
	node_ptr_t<KeyType, ValueType> remove_min(node_ptr_t<KeyType, ValueType> p); // ����������� ������������ ��������
	node_ptr_t<KeyType, ValueType> erase(node_ptr_t<KeyType, ValueType> p, const KeyType& input_key);	//����������� delete �������� key

public:
	// variables

	// functions

	// const
	node_ptr_t<KeyType, ValueType> begin() const;	// ��������� �� ������
	bool empty() const;	// ������� ��������� � �������
	unsigned int size() const;	//���������� ��������� � �������
	void print() const; // print �������
	node_ptr_t<KeyType, ValueType> find(const KeyType& input_key) const; // ����� ���� � �������� ������

	//not const
	Avl_tree() { root = nullptr; };	//������ �����������
	void insert(const KeyType& input_key, const ValueType& input_value);	// insert key �� ���������
	void insert(const KeyType& input_key);	// insert key ��� ��������
	Avl_tree(const std::initializer_list<std::pair<KeyType,ValueType>>& list): // ����������� � ������ �������������
		 Avl_tree() {	// ������������� ������������
		for (auto &element : list)
			insert(element.first, element.second);
	}
	void clear() {root = nullptr;} // ������� ������
	void erase(const KeyType& input_key);	// delete key
	ValueType& operator[](const KeyType& input_key);	// ���������� ��������� []
};

// ���������� ��������� []
template <typename KeyType, typename ValueType>
ValueType& Avl_tree<KeyType,ValueType>::operator[](const KeyType& input_key) {
	insert(input_key);
	return find(input_key)->value;
}

// ��������� �� ������ �������
template <typename KeyType, typename ValueType>
node_ptr_t<KeyType, ValueType> Avl_tree<KeyType, ValueType>::begin() const {
	return root;
}

//������� ��������� � ������
template <typename KeyType, typename ValueType>
bool Avl_tree<KeyType, ValueType>::empty() const {
	if (begin() != nullptr) {
		return false;
	} else {
		return true;
	}
}

//���-�� ��������� � ������
template <typename KeyType, typename ValueType>
unsigned int Avl_tree<KeyType, ValueType>::size() const {
	if(!empty()) {
		return begin()->children + 1;
	} else {
		return 0;
	}
}

//print ������
template <typename KeyType, typename ValueType>
void Avl_tree<KeyType, ValueType>::print() const {
	if (!empty()) {
		print(begin(), 0);
		std::cout << std::endl;
	} else {
		std::cout<< "Tree is empty" << std::endl;
	}
}

// recursive print ������
template <typename KeyType, typename ValueType>
void Avl_tree<KeyType, ValueType>::print(node_ptr_t<KeyType, ValueType> p, int level) const {
	if (p) {
		print(p->right, level + 1);
		for (int i = 0; i < level; i++) std::cout << "  ";
		std::cout << p->key << ";" << p->children << std::endl;
		print(p->left, level+1);
	}
}

// ������ ������ ������� ����� ���������
template <typename KeyType, typename ValueType>
void Avl_tree<KeyType, ValueType>::fixheight(node_ptr_t<KeyType, ValueType> p) {
	unsigned int hl = get_height(p->left);
	unsigned int hr = get_height(p->right);
	p->height = (hl>hr?hl:hr) + 1;
	unsigned int r = p->right == nullptr ? -1 : p->right->children;
	unsigned int l = p->left == nullptr ? -1 : p->left->children;
	p->children = r + l + 2;
}

// ������ �������
template <typename KeyType, typename ValueType>
node_ptr_t<KeyType, ValueType> Avl_tree<KeyType, ValueType>::rotateright(node_ptr_t<KeyType, ValueType> p) {
	node_ptr_t<KeyType, ValueType> q(p->left);
	p->left = q->right;
	q->right = p;
	fixheight(p);
	fixheight(q);
	return q;
}

//����� �������
template <typename KeyType, typename ValueType>
node_ptr_t<KeyType, ValueType> Avl_tree<KeyType, ValueType>::rotateleft(node_ptr_t<KeyType, ValueType> q) {
	node_ptr_t<KeyType, ValueType> p(q->right);
	q->right = p->left;
	p->left = q;
	fixheight(q);
	fixheight(p);
	return p;
}

// ������������ ����
template <typename KeyType, typename ValueType>
node_ptr_t<KeyType, ValueType> Avl_tree<KeyType, ValueType>::balance(node_ptr_t<KeyType, ValueType> p) {
	fixheight(p);
	switch (bfactor(p)) {
	case -2:
		if (bfactor(p->left) > 0) {
			p->left = rotateleft(p->left);
		}
		return rotateright(p);
	case 2:
		if (bfactor(p->right) < 0) {
			p->right = rotateright(p->right);
		}
		return rotateleft(p);
	default:
		return p;
	}
}

// ����� ���� � �������� ������
template <typename KeyType, typename ValueType>
node_ptr_t<KeyType, ValueType> Avl_tree<KeyType, ValueType>::find(const KeyType& input_key) const {
	return find(root, input_key);
}

// recursive ����� ���� � �������� ������
template <typename KeyType, typename ValueType>
node_ptr_t<KeyType, ValueType> Avl_tree<KeyType, ValueType>::find(node_ptr_t<KeyType, ValueType> p, const KeyType& input_key) const {
	if(!p) return nullptr;
	if( input_key < p->key )
		p->left = find(p->left, input_key);
	else if( input_key > p->key )
		p->right = find(p->right, input_key);
	return p;
}


// insert key ��� ��������
template <typename KeyType, typename ValueType>
void Avl_tree<KeyType, ValueType>::insert(const KeyType& input_key) {
	if (root != nullptr) {	// ���� �� ������� ������ ��� ����� ������, �� �������� ����������� �������
		root = insert(input_key, root);
	}
	else {	// ���� � ������ ��� �����, �� �������� ���
		//�������� ������ ��� ������ ���� � ������� ����������� � �������� ��������� �� ��� ������ � root
		root = std::make_shared<node<KeyType,ValueType>>(input_key);
	}
}

// insert key �� ���������
template <typename KeyType, typename ValueType>
void Avl_tree<KeyType, ValueType>::insert(const KeyType& input_key, const ValueType& input_value) {
	if (root != nullptr) {	// ���� �� ������� ������ ��� ����� ������, �� �������� ����������� �������
		root = insert(input_key, input_value, root);
	}
	else {	// ���� � ������ ��� �����, �� �������� ���
		//�������� ������ ��� ������ ���� � ������� ����������� � �������� ��������� �� ��� ������ � root
		root = std::make_shared<node<KeyType,ValueType>>(input_key, input_value);
	}
}

//recursive insert key ��� ��������
template <typename KeyType, typename ValueType>
node_ptr_t<KeyType, ValueType> Avl_tree<KeyType, ValueType>::insert(const KeyType& input_key, node_ptr_t<KeyType, ValueType> leaf) {
	if (input_key < leaf->key) {
		if (leaf->left != nullptr) {
			leaf->left = insert(input_key, leaf->left);
		}
		else {
			//�������� ������ ��� ������ ���� � ������� ����������� � �������� ��������� �� ��� ������ � leaf->left
			leaf->left = std::make_shared<node<KeyType,ValueType>>(input_key);
		}
	}
	else {
		if (leaf->right != nullptr) {
			leaf->right = insert(input_key, leaf->right);
		}
		else {
			//�������� ������ ��� ������ ���� � ������� ����������� � �������� ��������� �� ��� ������ � leaf->right
			leaf->right = std::make_shared<node<KeyType,ValueType>>(input_key);
		}
	}
	return  balance(leaf);
}

//recursive insert key �� ���������
template <typename KeyType, typename ValueType>
node_ptr_t<KeyType, ValueType> Avl_tree<KeyType, ValueType>::insert(const KeyType& input_key, const ValueType& input_value, node_ptr_t<KeyType, ValueType> leaf) {
	if (input_key < leaf->key) {
		if (leaf->left != nullptr) {
			leaf->left = insert(input_key, input_value, leaf->left);
		}
		else {
			//�������� ������ ��� ������ ���� � ������� ����������� � �������� ��������� �� ��� ������ � leaf->left
			leaf->left = std::make_shared<node<KeyType,ValueType>>(input_key, input_value);
		}
	}
	else {
		if (leaf->right != nullptr) {
			leaf->right = insert(input_key, input_value, leaf->right);
		}
		else {
			//�������� ������ ��� ������ ���� � ������� ����������� � �������� ��������� �� ��� ������ � leaf->right
			leaf->right = std::make_shared<node<KeyType,ValueType>>(input_key, input_value);
		}
	}
	return  balance(leaf);
}

// delete key �� ���������
template <typename KeyType, typename ValueType>
void Avl_tree<KeyType,ValueType>::erase(const KeyType& input_key) {
	root = erase(root, input_key);
}

//delete ���� � ����������� ������ �� ��������� p
template <typename KeyType, typename ValueType>
node_ptr_t<KeyType, ValueType> Avl_tree<KeyType,ValueType>::remove_min(node_ptr_t<KeyType, ValueType> p) {
	if(p->left == nullptr)
		return p->right;
	p->left = remove_min(p->left);
	return balance(p);
}

// ����������� delete key
template <typename KeyType, typename ValueType>
node_ptr_t<KeyType, ValueType> Avl_tree<KeyType,ValueType>::erase(node_ptr_t<KeyType, ValueType> p, const KeyType& input_key) {
	if(!p) return 0;
	if( input_key < p->key )
		p->left = erase(p->left, input_key);
	else if( input_key > p->key )
		p->right = erase(p->right, input_key);
	else {
		node_ptr_t<KeyType, ValueType> q(p->left);
		node_ptr_t<KeyType, ValueType> r(p->right);
		if(!r) return q;
		node_ptr_t<KeyType, ValueType> min = find_min(r);
		min->right = remove_min(r);
		min->left = q;
		return balance(min);
	}
	return balance(p);
}

#endif // AVL_TREE_H
