#ifndef AVL_TREE_H
#define AVL_TREE_H

#include "node.h"
#include<memory>
#include<iostream>
#include <initializer_list>


template <class T1, class T2>
class Avl_tree {
private:
	//переменные
	std::shared_ptr<node<T1,T2>> root;

	// функции

	// const
	void print(std::shared_ptr<node<T1,T2>> p, int level) const; // рекурсивная печать дерева
	unsigned int get_height(std::shared_ptr<node<T1,T2>> p) const { return p != nullptr ? p->height : 0; }	// корректный геттер "высоты" узла
	int bfactor(std::shared_ptr<node<T1,T2>> p) const { return get_height(p->right) - get_height(p->left); }	// фактор балансировки узла
	std::shared_ptr<node<T1,T2>> find_min(std::shared_ptr<node<T1,T2>> p) const { return p->left ? find_min(p->left) : p; }	// поиск минимального ключа в поддереве данного узла
	std::shared_ptr<node<T1,T2>> find(std::shared_ptr<node<T1,T2>> p, const T1& input_key) const; // рекурсивный поиск узла с заданным ключом

	//not const
	std::shared_ptr<node<T1,T2>> insert(const T1& input_key, const T2& input_value, std::shared_ptr<node<T1,T2>> leaf);	// рекурсивная вставка ключа со значением
	std::shared_ptr<node<T1,T2>> insert(const T1& input_key, std::shared_ptr<node<T1,T2>> leaf);	// рекурсивная вставка ключа без значения
	void fixheight(std::shared_ptr<node<T1,T2>> p);	// апдейт высоты узла
	std::shared_ptr<node<T1,T2>> rotateright(std::shared_ptr<node<T1,T2>> p); // правый поворот вокруг p
	std::shared_ptr<node<T1,T2>> rotateleft (std::shared_ptr<node<T1,T2>> q); // левый поворот вокруг q
	std::shared_ptr<node<T1,T2>> balance(std::shared_ptr<node<T1,T2>> p); // балансировка узла p
	std::shared_ptr<node<T1,T2>> remove_min(std::shared_ptr<node<T1,T2>> p); // перемещение минимального элемента
	std::shared_ptr<node<T1,T2>> erase(std::shared_ptr<node<T1,T2>> p, const T1& input_key);	//рекурсивное удаление значения ключа

public:
	// переменные

	// функции

	// const
	std::shared_ptr<node<T1,T2>> begin() const;	// указатель на корень
	bool empty() const;	// наличие элементов в массиве
	unsigned int count() const;	//количество элементов в массиве
	void print() const; // печать массива
	std::shared_ptr<node<T1,T2>> find(const T1& input_key) const; // поиск узла с заданным ключом

	//not const
	Avl_tree() { root = nullptr; };	//пустой конструктор
	void insert(const T1& input_key, const T2& input_value);	// вставка ключа со значением
	void insert(const T1& input_key);	// вставка ключа без значения
	Avl_tree(const std::initializer_list<std::pair<T1,T2>>& list): // конструктор с листом инициализации
		 Avl_tree() {	// делегирование обязанностей
		for (auto &element : list)
			insert(element.first, element.second);
	}
	void clear() {root = nullptr;} // очистка дерева
	void erase(const T1& input_key);	// удаление ключа
	T2& operator[](const T1& input_key);	// перегрузка оператора []
};

// перегрузка оператора []
template <typename T1, typename T2>
T2& Avl_tree<T1,T2>::operator[](const T1& input_key) {
	insert(input_key);
	return find(input_key)->value;
}

// указатель на первый элемент
template <typename T1, typename T2>
std::shared_ptr<node<T1,T2>> Avl_tree<T1, T2>::begin() const {
	return root;
}

//наличие элементов в дереве
template <typename T1, typename T2>
bool Avl_tree<T1, T2>::empty() const {
	if (begin() != nullptr) {
		return false;
	} else {
		return true;
	}
}

//кол-во элементов в дереве
template <typename T1, typename T2>
unsigned int Avl_tree<T1, T2>::count() const {
	if(!empty()) {
		return begin()->children + 1;
	} else {
		return 0;
	}
}

//печать дерева
template <typename T1, typename T2>
void Avl_tree<T1, T2>::print() const {
	if (!empty()) {
		print(begin(), 0);
		std::cout << std::endl;
	} else {
		std::cout<< "Tree is empty" << std::endl;
	}
}

// рекурсивная печать дерева
template <typename T1, typename T2>
void Avl_tree<T1, T2>::print(std::shared_ptr<node<T1,T2>> p, int level) const {
	if (p) {
		print(p->right, level + 1);
		for (int i = 0; i < level; i++) std::cout << "  ";
		std::cout << p->key << ";" << p->children << std::endl;
		print(p->left, level+1);
	}
}

// апдейт высоты вершины после поворотов
template <typename T1, typename T2>
void Avl_tree<T1, T2>::fixheight(std::shared_ptr<node<T1,T2>> p) {
	unsigned int hl = get_height(p->left);
	unsigned int hr = get_height(p->right);
	p->height = (hl>hr?hl:hr) + 1;
	unsigned int r = p->right == nullptr ? -1 : p->right->children;
	unsigned int l = p->left == nullptr ? -1 : p->left->children;
	p->children = r + l + 2;
}

// правый поворот
template <typename T1, typename T2>
std::shared_ptr<node<T1,T2>> Avl_tree<T1, T2>::rotateright(std::shared_ptr<node<T1,T2>> p) {
	std::shared_ptr<node<T1,T2>> q(p->left);
	p->left = q->right;
	q->right = p;
	fixheight(p);
	fixheight(q);
	return q;
}

//левый поворот
template <typename T1, typename T2>
std::shared_ptr<node<T1,T2>> Avl_tree<T1, T2>::rotateleft(std::shared_ptr<node<T1,T2>> q) {
	std::shared_ptr<node<T1,T2>> p(q->right);
	q->right = p->left;
	p->left = q;
	fixheight(q);
	fixheight(p);
	return p;
}

// балансировка узла
template <typename T1, typename T2>
std::shared_ptr<node<T1,T2>> Avl_tree<T1, T2>::balance(std::shared_ptr<node<T1,T2>> p) {
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
template <typename T1, typename T2>
std::shared_ptr<node<T1,T2>> Avl_tree<T1, T2>::find(const T1& input_key) const {
	return find(root, input_key);
}

// рекурсивный поиск узла с заданным ключом
template <typename T1, typename T2>
std::shared_ptr<node<T1,T2>> Avl_tree<T1, T2>::find(std::shared_ptr<node<T1,T2>> p, const T1& input_key) const {
	if(!p) return nullptr;
	if( input_key < p->key )
		p->left = find(p->left, input_key);
	else if( input_key > p->key )
		p->right = find(p->right, input_key);
	return p;
}


// вставка ключа без значения
template <typename T1, typename T2>
void Avl_tree<T1, T2>::insert(const T1& input_key) {
	if (root != nullptr) {	// если по данному адресу уже лежит объект, то запустим рекурсивную вставку
		root = insert(input_key, root);
	}
	else {	// если в дереве нет корня, то создадим его
		//выделяем память для нового узла с данными параметрами и передаем указатель на эту память в root
		root = std::make_shared<node<T1,T2>>(input_key);
	}
}

// вставка ключа со значением
template <typename T1, typename T2>
void Avl_tree<T1, T2>::insert(const T1& input_key, const T2& input_value) {
	if (root != nullptr) {	// если по данному адресу уже лежит объект, то запустим рекурсивную вставку
		root = insert(input_key, input_value, root);
	}
	else {	// если в дереве нет корня, то создадим его
		//выделяем память для нового узла с данными параметрами и передаем указатель на эту память в root
		root = std::make_shared<node<T1,T2>>(input_key, input_value);
	}
}

//рекурсивная вставка ключа без значения
template <typename T1, typename T2>
std::shared_ptr<node<T1,T2>> Avl_tree<T1, T2>::insert(const T1& input_key, std::shared_ptr<node<T1,T2>> leaf) {
	if (input_key < leaf->key) {
		if (leaf->left != nullptr) {
			leaf->left = insert(input_key, leaf->left);
		}
		else {
			//выделяем память для нового узла с данными параметрами и передаем указатель на эту память в leaf->left
			leaf->left = std::make_shared<node<T1,T2>>(input_key);
		}
	}
	else {
		if (leaf->right != nullptr) {
			leaf->right = insert(input_key, leaf->right);
		}
		else {
			//выделяем память для нового узла с данными параметрами и передаем указатель на эту память в leaf->right
			leaf->right = std::make_shared<node<T1,T2>>(input_key);
		}
	}
	return  balance(leaf);
}

//рекурсивная вставка ключа со значением
template <typename T1, typename T2>
std::shared_ptr<node<T1,T2>> Avl_tree<T1, T2>::insert(const T1& input_key, const T2& input_value, std::shared_ptr<node<T1,T2>> leaf) {
	if (input_key < leaf->key) {
		if (leaf->left != nullptr) {
			leaf->left = insert(input_key, input_value, leaf->left);
		}
		else {
			//выделяем память для нового узла с данными параметрами и передаем указатель на эту память в leaf->left
			leaf->left = std::make_shared<node<T1,T2>>(input_key, input_value);
		}
	}
	else {
		if (leaf->right != nullptr) {
			leaf->right = insert(input_key, input_value, leaf->right);
		}
		else {
			//выделяем память для нового узла с данными параметрами и передаем указатель на эту память в leaf->right
			leaf->right = std::make_shared<node<T1,T2>>(input_key, input_value);
		}
	}
	return  balance(leaf);
}

// удаление ключа со значением
template <typename T1, typename T2>
void Avl_tree<T1,T2>::erase(const T1& input_key) {
	root = erase(root, input_key);
}

//удаление узла с минимальным ключом из поддерева p
template <typename T1, typename T2>
std::shared_ptr<node<T1,T2>> Avl_tree<T1,T2>::remove_min(std::shared_ptr<node<T1,T2>> p) {
	if(p->left == nullptr)
		return p->right;
	p->left = remove_min(p->left);
	return balance(p);
}

// рекурсивное удаление ключа
template <typename T1, typename T2>
std::shared_ptr<node<T1,T2>> Avl_tree<T1,T2>::erase(std::shared_ptr<node<T1,T2>> p, const T1& input_key) {
	if(!p) return 0;
	if( input_key < p->key )
		p->left = erase(p->left, input_key);
	else if( input_key > p->key )
		p->right = erase(p->right, input_key);
	else {
		std::shared_ptr<node<T1,T2>> q(p->left);
		std::shared_ptr<node<T1,T2>> r(p->right);
		if(!r) return q;
		std::shared_ptr<node<T1,T2>> min = find_min(r);
		min->right = remove_min(r);
		min->left = q;
		return balance(min);
	}
	return balance(p);
}

#endif // AVL_TREE_H
