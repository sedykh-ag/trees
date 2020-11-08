#ifndef NODE_H
#define NODE_H
#include<memory>
#include<iostream>


template <class T1, class T2>
struct node {	// ��������� ��� ������������� ���� ������
	const T1 key;	//���� ���� T2
	T2 value;	//�������� ���� T2
	std::shared_ptr<node<T1,T2>> left;	//��������� �� ������ �������
	std::shared_ptr<node<T1,T2>> right;	//��������� �� ������� �������
	unsigned int height;	//������ ��������� � ������ � ������ ����
	unsigned int children;	// ���-�� �����
	bool is_value;

	node(const T1& input_key, const T2& input_value) : // ����������� ���� � ������ � ���������
		 key{input_key}, value{input_value} { // ������ �������������
		 	left  = nullptr;
		 	right = nullptr;
		 	height = 1;
		 	children = 0;
		 	is_value = true;
	}
	node(const T1& input_key) : // ����������� ���� � ������ ��� ��������
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
