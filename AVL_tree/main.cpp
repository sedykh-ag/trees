#include<iostream>
#include<string>
#include<queue>
#include "Avl_tree.h"
#include "node.h"
using namespace std;


int main() {
	// ������������� ������
	Avl_tree <int, string> tr {{50, "word-50"}, {50, "word-50x"}, {10, "word-10"}, {20,"word-20"},
	 {14, "word-14"}, {15,"word-15"}, {30, "word-30"} , {35,"word-35"} , {40, "word-40"}};
	// print ������
	cout<<"Tree:" << endl;
	tr.print();
	cout <<"tree size is: " << tr.size() << endl;
	if (tr.empty()) {
		cout << "tree is empty" << endl;
	}
	else {
		cout << "tree is not empty" << endl;
	}

	// �������� �������� �� �����
	cout<<"Erase 14:" << endl;
	tr.erase(14);
	tr.print();
	cout << "Clear Tree:" << endl;
	tr.clear();
	cout <<"tree size is: " << tr.size() << endl;
	if (tr.empty()) {
		cout << "tree is empty" << endl;
	}
	else {
		cout << "tree is not empty" << endl;
	}
	tr.insert(50, "word-50");
	cout << "find 50: " << tr.find(50)->value << endl;
	tr[40] = "word-40";
	cout << "find 40: " << tr.find(40)->value << endl;
	cout<<"Tree:" << endl;
	tr.print();

	return 0;
}
