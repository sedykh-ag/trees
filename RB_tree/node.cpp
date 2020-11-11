#include<bits/stdc++.h>

// BASIC TYPE DEFINITIONS

enum color_t { BLACK, RED };

template<class Key, class T>
struct Node {
    // vars
    Node* parent, left, right;
    enum color_t color;
    Key key;
    T value;

    Node(Key key_o, T value_o, color_t color_o = RED) :  key(key_o),
                                                         value(value_o),
                                                         color(color_o),
                                                         left(nullptr),
                                                         right(nullptr),
                                                         parent(nullptr)
    { }
};

