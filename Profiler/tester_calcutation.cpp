#include<iostream>
#include<utility>
#include <chrono>
#include <cmath>

#include "../AVL_tree/Avl_tree.h"
#include "../RB_tree/Rb_tree.cpp"


double get_time()
{
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count()/1e6;
}

int main() {

    Avl_tree<int, int> test_AVL_tree;
    RBTree<int> test_RB_tree;

    double time, start, finish;

    int rep = 100;
    int max_N = 1000000;
    int pow;
    int N;

    for (int t = 100; t <= max_N; t *= 10){
        pow = t;
        N = 0;

        test_AVL_tree.clear();
        for (int i = 0; i < pow; i++){
                test_AVL_tree.insert(i);
            }

        for (int j = 0; j < 100; j++){
            if(j > 9){
                for (int i = pow * j + rep * j; i < pow * (j+1) + rep * j; i++){
                    //Choose one:

                    //test_AVL_tree.insert(i);
                    //test_RB_tree.insert(i);
                }
            }
            start = get_time();
            for (int i = pow * (j+1) + rep * j; i < (pow + rep) * (j + 1); i++){
                //Choose one:

                //test_AVL_tree.insert(i);
                //test_AVL_tree.find(i % 10);
                //test_AVL_tree.erase(i);

                //test_RB_tree.insert(i);
                //test_RB_tree.find(i);
                //test_RB_tree.remove(i);
            }
            finish = get_time();
            time = finish - start;
            N += pow;
            std::cout << N <<", " << time << std::endl;
        }
    }
}
