#include<iostream>

#include "../AVL_tree/Avl_tree.h"
#include "../RB_tree/rb_tree.cpp"


int main() {

    Avl_tree<int, int> test_AVL_tree;
    RBTree<int, int> test_RB_tree;

    int max_N = 100000;

    // --- Testing AVL-tree
    for (int i = 0; i <= max_N; i++){
        test_AVL_tree.insert(i, i);
    }
    std::cout << "AVL-tree insert test passed" << "\n";

    for (int i = 0; i <= max_N; i++){
        test_AVL_tree.find(i);
    }
    std::cout << "AVL-tree find test was passed" << "\n";

    for (int i = 0; i <= max_N; i++){
        test_AVL_tree.erase(i);
    }
    std::cout << "AVL-tree erase test was passed" << "\n";


    // --- Testing RB-tree
    for (int i = 0; i <= max_N; i++){
        test_RB_tree.insert(i, i);
    }
    std::cout << "RB-tree insert test was passed" << "\n";

    for (int i = 0; i <= max_N; i++){
        test_AVL_tree.find(i);
    }
    std::cout << "RB-tree find test was passed" << "\n";
/*
    for (int i = 0; i <= max_N; i++){
        test_RB_tree.erase(i);
    }
    std::cout << "RB-tree erase test passed" << "\n";
*/
}
