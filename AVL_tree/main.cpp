#include<iostream>
#include<string>
#include<queue>
#include "Avl_tree.h"
#include "node.h"
using namespace std;


int main() {
	// инициализация дерева
	Avl_tree <int, string> tr {{50, "word-50"}, {9, "word-9"}, {10, "word-10"}, {20,"word-20"},
	 {14, "word-14"}, {15,"word-15"}, {30, "word-30"} , {35,"word-35"} , {40, "word-40"}};
	// печать дерева
	cout<<"Tree:" << endl;
	tr.print();
	cout <<"tree count is: " << tr.count() << endl;
	if (tr.empty()) {
		cout << "tree is empty" << endl;
	}
	else {
		cout << "tree is not empty" << endl;
	}

	// удаление элемента по ключу
	cout<<"Erase 14:" << endl;
	tr.erase(14);
	tr.print();

	tr.clear();
	cout <<"tree count is: " << tr.count() << endl;
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
	return 0;
}
