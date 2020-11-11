#ifndef NODE_H
#define NODE_H
#include<memory>
#include<iostream>

template <class KeyType, class ValueType>
struct node {	// структура для представления узла дерева
	const KeyType key;	//ключ типа ValueType
	ValueType value;	//значение типа ValueType
	std::shared_ptr<node<KeyType,ValueType>> left;	//указатель на левого потомка
	std::shared_ptr<node<KeyType,ValueType>> right;	//указатель на правого потомка
	std::weak_ptr<node<KeyType,ValueType>> parent;	//указатель на правого потомка
	unsigned int height;	//высота поддерева с корнем в данном узле
	unsigned int children;	// кол-во детей
	bool is_value;

	node(const KeyType& input_key, const ValueType& input_value, std::shared_ptr<node<KeyType,ValueType>> p) : // конструктор узла с ключом и значением
		 key{input_key}, value{input_value}, parent(p) { // список инициализации
		 	left  = nullptr;
		 	right = nullptr;
		 	height = 1;
		 	children = 0;
		 	is_value = true;
	}
	node(const KeyType& input_key, std::shared_ptr<node<KeyType,ValueType>> p) : // конструктор узла с ключом без значения
		 key{input_key}, parent(p) { // список инициализации
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
