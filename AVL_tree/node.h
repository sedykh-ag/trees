#ifndef NODE_H
#define NODE_H
#include<memory>
#include<iostream>

template <class KeyType, class ValueType>
struct node {	// ��������� ��� ������������� ���� ������
	const KeyType key;	//���� ���� ValueType
	ValueType value;	//�������� ���� ValueType
	std::shared_ptr<node<KeyType,ValueType>> left;	//��������� �� ������ �������
	std::shared_ptr<node<KeyType,ValueType>> right;	//��������� �� ������� �������
	unsigned int height;	//������ ��������� � ������ � ������ ����
	unsigned int children;	// ���-�� �����
	bool is_value;

	node(const KeyType& input_key, const ValueType& input_value) : // ����������� ���� � ������ � ���������
		 key{input_key}, value{input_value} { // ������ �������������
		 	left  = nullptr;
		 	right = nullptr;
		 	height = 1;
		 	children = 0;
		 	is_value = true;
	}
	node(const KeyType& input_key) : // ����������� ���� � ������ ��� ��������
		 key{input_key} { // ������ �������������
		 	left  = nullptr;
		 	right = nullptr;
		 	height = 1;
		 	children = 0;
		 	is_value = false;
	}
	~node() { // ����������
		std::cout << "node key: " << key << " deleted" << std::endl;
	}
};
#endif
