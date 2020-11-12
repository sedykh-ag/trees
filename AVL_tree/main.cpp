#include<iostream>
#include<string>
#include<queue>
#include "Avl_tree.h"
#include "node.h"
using namespace std;


int main() {
	// initialization
	Avl_tree <int, string> tr {{50, "word-50"}, {4, "word-4"}, {27, "word-27"}, {9, "word-9"}, {10, "word-10"}, {20,"word-20"}, {14, "word-14"}, {15,"word-15"}, {30, "word-30"} , {35,"word-35"} , {40, "word-40"}};
	// print tree
	cout<<"Tree:" << endl;
	tr.print();
	tr.insert(17);
	cout << endl;
	cout<<"Tree:" << endl;
	tr.print();
	cout <<"tree size is: " << tr.size() << endl;
	if (tr.empty()) cout << "tree is empty" << endl;
	else cout << "tree is not empty" << endl;
	cout<< "Printing our tree with iterators:" << endl;
	for (auto i = tr.begin(); i != tr.end(); ++i) {
		cout << i->key << " ";
	}
	cout <<endl << "Reversive print with iterators:" << endl;
	auto i = tr.end();
	--i;
	for (i; i != tr.begin(); --i) {
		cout << i->key << " ";
	}
	cout << (tr.begin())->key << endl;
	cout<<endl<<"Erase 50:" << endl;
	tr.erase(50);
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
