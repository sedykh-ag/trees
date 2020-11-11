#ifndef AVL_TREE_H
#define AVL_TREE_H

#include "node.h"

#include<memory>
#include<iostream>
#include"Avl_tree_iterator.h"
#include <initializer_list>



template <class KeyType, class ValueType>
class Avl_tree {
public:
	typedef std::shared_ptr<node<KeyType,ValueType>> node_ptr_t;
	typedef Avl_tree_iterator<KeyType,ValueType,node_ptr_t> iterator;
	typedef Avl_tree_iterator<const KeyType, const ValueType, const node_ptr_t> const_iterator;
	friend class Avl_tree_iterator<KeyType,ValueType,node_ptr_t>;
	Avl_tree* parent_class = this;
private:
	//variables
	node_ptr_t root;

	// functions

	// const
	void print(node_ptr_t p, int level) const; // recursive print of tree
	unsigned int get_height(node_ptr_t p) const { return p != nullptr ? p->height : 0; }	// getter height of node
	int bfactor(node_ptr_t p) const { return get_height(p->right) - get_height(p->left); }	// node's balance factor
	node_ptr_t find_min(node_ptr_t p) const { return p->left ? find_min(p->left) : p; }	// search for the minimum key of a given node
	node_ptr_t find_max(node_ptr_t p) const { return p->right ? find_max(p->right) : p; }	// search for the maximum key of a given node
	node_ptr_t find(node_ptr_t p, const KeyType& input_key) const; // recursive поиск узла с заданным ключом

	//not const
	node_ptr_t insert(const KeyType& input_key, const ValueType& input_value, node_ptr_t leaf);	// recursive insert key со значением
	node_ptr_t insert(const KeyType& input_key, node_ptr_t leaf);	// recursive insert key без значения
	void fixheight(node_ptr_t p);	// апдейт высоты узла
	node_ptr_t rotateright (node_ptr_t p); // правый поворот вокруг p
	node_ptr_t rotateleft (node_ptr_t q); // левый поворот вокруг q
	node_ptr_t balance(node_ptr_t p); // балансировка узла p
	node_ptr_t remove_min(node_ptr_t p); // перемещение минимального элемента
	node_ptr_t erase(node_ptr_t p, const KeyType& input_key);	//рекурсивное delete значения key
	node_ptr_t next_node (node_ptr_t p);
	node_ptr_t prev_node (node_ptr_t p);

public:
	// variables

	// functions

	// const
	iterator begin();	// iterator to begin
	iterator end();	// iterator to end
	bool empty() const;	// наличие элементов в массиве
	unsigned int size() const;	//количество элементов в массиве
	void print() const; // print массива
	node_ptr_t find(const KeyType& input_key) const; // поиск узла с заданным ключом

	//not const
	Avl_tree() { root = nullptr;};	//пустой конструктор
	void insert(const KeyType& input_key, const ValueType& input_value);	// insert key со значением
	void insert(const KeyType& input_key);	// insert key без значения
	Avl_tree(const std::initializer_list<std::pair<KeyType,ValueType>>& list): // конструктор с листом инициализации
		Avl_tree() {	// делегирование обязанностей
		for (auto &element : list)
			insert(element.first, element.second);
	}
	void clear() {root = nullptr;} // очистка дерева
	void erase(const KeyType& input_key);	// delete key
	ValueType& operator[](const KeyType& input_key);	// перегрузка оператора []

};

// перегрузка оператора []
template <typename KeyType, typename ValueType>
ValueType& Avl_tree<KeyType,ValueType>::operator[](const KeyType& input_key) {
	insert(input_key);
	return find(input_key)->value;
}

// iterator to begin
template <typename KeyType, typename ValueType>
typename Avl_tree<KeyType, ValueType>::iterator Avl_tree<KeyType, ValueType>::begin() {
	return typename Avl_tree<KeyType, ValueType>::iterator(find_min(root), parent_class);
}

// iterator to end
template <typename KeyType, typename ValueType>
typename Avl_tree<KeyType, ValueType>::iterator Avl_tree<KeyType, ValueType>::end() {
	return typename Avl_tree<KeyType, ValueType>::iterator(find_max(root), parent_class);
}


//наличие элементов в дереве
template <typename KeyType, typename ValueType>
bool Avl_tree<KeyType, ValueType>::empty() const {
	return ((root != nullptr) ? false : true);
}

//кол-во элементов в дереве
template <typename KeyType, typename ValueType>
unsigned int Avl_tree<KeyType, ValueType>::size() const {
	return ((!empty()) ? root->children + 1 : 0);
}

//print дерева
template <typename KeyType, typename ValueType>
void Avl_tree<KeyType, ValueType>::print() const {
	std::cout << std::endl;
	if (!empty()) print(root, 0);
	else std::cout<< "Tree is empty";
	std::cout << std::endl;
}

// recursive print дерева
template <typename KeyType, typename ValueType>
void Avl_tree<KeyType, ValueType>::print(node_ptr_t p, int level) const {
	if (p) {
		print(p->right, level + 1);
		for (int i = 0; i < level; i++) std::cout << "  ";
		std::cout << p->key << ";" << p->children << std::endl;
		print(p->left, level+1);
	}
}

// апдейт высоты вершины после поворотов
template <typename KeyType, typename ValueType>
void Avl_tree<KeyType, ValueType>::fixheight(node_ptr_t p) {
	unsigned int hl = get_height(p->left);
	unsigned int hr = get_height(p->right);
	p->height = (hl>hr?hl:hr) + 1;
	unsigned int r = p->right == nullptr ? -1 : p->right->children;
	unsigned int l = p->left == nullptr ? -1 : p->left->children;
	p->children = r + l + 2;
}

// right rotate
template <typename KeyType, typename ValueType>
typename Avl_tree<KeyType, ValueType>::node_ptr_t Avl_tree<KeyType, ValueType>::rotateright(node_ptr_t p) {
	node_ptr_t q(p->left);
	p->left = q->right;
	q->right = p;

	(q->right)->parent = p;
	q->parent = p->parent;
	p->parent = q;
	fixheight(p);
	fixheight(q);
	return q;
}

// left rotate
template <typename KeyType, typename ValueType>
typename Avl_tree<KeyType, ValueType>::node_ptr_t Avl_tree<KeyType, ValueType>::rotateleft(node_ptr_t q) {
	node_ptr_t p(q->right);
	q->right = p->left;
	p->left = q;

	p->parent = q->parent;
	(p->left)->parent = q;
	q->parent = p;
	fixheight(q);
	fixheight(p);
	return p;
}

// балансировка узла
template <typename KeyType, typename ValueType>
typename Avl_tree<KeyType, ValueType>::node_ptr_t Avl_tree<KeyType, ValueType>::balance(node_ptr_t p) {
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

// поиск узла с заданным ключом
template <typename KeyType, typename ValueType>
typename Avl_tree<KeyType, ValueType>::node_ptr_t Avl_tree<KeyType, ValueType>::find(const KeyType& input_key) const {
	return find(root, input_key);
}

// recursive поиск узла с заданным ключом
template <typename KeyType, typename ValueType>
typename Avl_tree<KeyType, ValueType>::node_ptr_t Avl_tree<KeyType, ValueType>::find(node_ptr_t p, const KeyType& input_key) const {
	if(!p) return nullptr;
	if( input_key < p->key )
		p->left = find(p->left, input_key);
	else if( input_key > p->key )
		p->right = find(p->right, input_key);
	return p;
}


// insert key без значения
template <typename KeyType, typename ValueType>
void Avl_tree<KeyType, ValueType>::insert(const KeyType& input_key) {
	root = (root == nullptr) ? std::make_shared<node<KeyType,ValueType>>(input_key, nullptr) : insert(input_key, root);
}

// insert key со значением
template <typename KeyType, typename ValueType>
void Avl_tree<KeyType, ValueType>::insert(const KeyType& input_key, const ValueType& input_value) {
	root = (root == nullptr) ? std::make_shared<node<KeyType,ValueType>>(input_key, input_value, nullptr) : insert(input_key, input_value, root);
}

//recursive insert key без значения
template <typename KeyType, typename ValueType>
typename Avl_tree<KeyType, ValueType>::node_ptr_t Avl_tree<KeyType, ValueType>::insert(const KeyType& input_key, node_ptr_t leaf) {
	if (input_key < leaf->key)
		leaf->left  = (leaf->left  == nullptr) ? std::make_shared<node<KeyType,ValueType>>(input_key, leaf) : insert(input_key, leaf->left );
	else if (input_key > leaf->key)
		leaf->right = (leaf->right == nullptr) ? std::make_shared<node<KeyType,ValueType>>(input_key, leaf) : insert(input_key, leaf->right);

	return  balance(leaf);
}

//recursive insert key со значением
template <typename KeyType, typename ValueType>
typename Avl_tree<KeyType, ValueType>::node_ptr_t Avl_tree<KeyType, ValueType>::insert(const KeyType& input_key, const ValueType& input_value, node_ptr_t leaf) {
	if (input_key < leaf->key)
		leaf->left  = (leaf->left  == nullptr) ? std::make_shared<node<KeyType,ValueType>>(input_key, input_value, leaf) : insert(input_key, input_value, leaf->left);
	else if (input_key > leaf->key)
		leaf->right = (leaf->right == nullptr) ? std::make_shared<node<KeyType,ValueType>>(input_key, input_value, leaf) : insert(input_key, input_value, leaf->right);
	else if (input_key == leaf->key)
		leaf->value = input_value;
	return  balance(leaf);
}

// delete key со значением
template <typename KeyType, typename ValueType>
void Avl_tree<KeyType,ValueType>::erase(const KeyType& input_key) {
	root = erase(root, input_key);
}

//delete узла с минимальным ключом из поддерева p
template <typename KeyType, typename ValueType>
typename Avl_tree<KeyType, ValueType>::node_ptr_t Avl_tree<KeyType,ValueType>::remove_min(node_ptr_t p) {
	if(p->left == nullptr) {
		if (p->right != nullptr) (p->right)->parent = p->parent;
		return p->right;
	}
	p->left = remove_min(p->left);
	return balance(p);
}

// рекурсивное delete key
template <typename KeyType, typename ValueType>
typename Avl_tree<KeyType, ValueType>::node_ptr_t Avl_tree<KeyType,ValueType>::erase(node_ptr_t p, const KeyType& input_key) {
	if(!p) return 0;
	if( input_key < p->key )
		p->left = erase(p->left, input_key);
	else if( input_key > p->key )
		p->right = erase(p->right, input_key);
	else {
		node_ptr_t q(p->left);
		node_ptr_t r(p->right);
		if(!r) {
			q->parent = p->parent;
			return q;
		}
		node_ptr_t min(find_min(r));
		min->right = remove_min(r);
		min->left = q;
		q->parent = min;
		(min->right)->parent = min;
		min->parent = p->parent;
		return balance(min);
	}
	return balance(p);
}

//next node
template <typename KeyType, typename ValueType>
typename Avl_tree<KeyType, ValueType>::node_ptr_t Avl_tree<KeyType, ValueType>::next_node (node_ptr_t p){
	if (p == end()) return p;
	if (p->parent != nullptr) {
		if (p == (p->parent)->left) {
			if ((p->parent)->right != nullptr) return find_min((p->parent)->right);
			else return p->parent;
		}
		else {
			if (p->right == nullptr) return find_min(((p->parent)->parent)->right);
			else return find_min(p->right);
		}
	}
	else {
		return find_min(p->right);
	}
}

/*
//prev node
template <typename KeyType, typename ValueType>
typename Avl_tree<KeyType, ValueType>::node_ptr_t Avl_tree<KeyType, ValueType>::prev_node (node_ptr_t p){

}
*/

#endif // AVL_TREE_H
