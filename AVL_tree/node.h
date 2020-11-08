#ifndef NODE_H
#define NODE_H
#include<memory>
#include<iostream>


template <class T1, class T2>
struct node {	// структура для представления узла дерева
	const T1 key;	//ключ типа T2
	T2 value;	//значение типа T2
	std::shared_ptr<node<T1,T2>> left;	//указатель на левого потомка
	std::shared_ptr<node<T1,T2>> right;	//указатель на правого потомка
	unsigned int height;	//высота поддерева с корнем в данном узле
	unsigned int children;	// кол-во детей
	bool is_value;

	node(const T1& input_key, const T2& input_value) : // конструктор узла с ключом и значением
		 key{input_key}, value{input_value} { // список инициализации
		 	left  = nullptr;
		 	right = nullptr;
		 	height = 1;
		 	children = 0;
		 	is_value = true;
	}
	node(const T1& input_key) : // конструктор узла с ключом без значения
		 key{input_key} { // список инициализации
		 	left  = nullptr;
		 	right = nullptr;
		 	height = 1;
		 	children = 0;
		 	is_value = false;
	}
	~node() { // деструктор
		std::cout << "node key: " << key << " deleted" << std::endl;
	}
};
#endif
