#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>


enum class Color {
    Red,
    Black
};

template <typename T, typename V>
struct Node {
    T key;
    V value;
    Color color;
    std::unique_ptr<Node<T, V>> left;
    std::unique_ptr<Node<T, V>> right;
    Node<T, V>* parent;

    Node(const T& key, const V& val) : key {key}, value {val}, color {Color::Red}, parent {nullptr} {}
};

template <typename T, typename V>
struct RBTree {
public:
    std::unique_ptr<Node<T, V>> root;

private:
    void LeftRotate(std::unique_ptr<Node<T, V>>&& x) {
        auto y = std::move(x->right);
        x->right = std::move(y->left);
        if (x->right) {
            x->right->parent = x.get();
        }
        y->parent = x->parent;
        auto xp = x->parent;
        if (!xp) {
            auto px = x.release();
            root = std::move(y);
            root->left = std::unique_ptr<Node<T, V>>(px);
            root->left->parent = root.get();
        } else if (x == xp->left) {
            auto px = x.release();
            xp->left = std::move(y);
            xp->left->left = std::unique_ptr<Node<T, V>>(px);
            xp->left->left->parent = xp->left.get();
        } else {
            auto px = x.release();
            xp->right = std::move(y);
            xp->right->left = std::unique_ptr<Node<T, V>>(px);
            xp->right->left->parent = xp->right.get();
        }
    }

    void RightRotate(std::unique_ptr<Node<T, V>>&& x) {
        auto y = std::move(x->left);
        x->left = std::move(y->right);
        if (x->left) {
            x->left->parent = x.get();
        }
        y->parent = x->parent;
        auto xp = x->parent;
        if (!xp) {
            auto px = x.release();
            root = std::move(y);
            root->right = std::unique_ptr<Node<T, V>>(px);
            root->right->parent = root.get();
        } else if (x == xp->left) {
            auto px = x.release();
            xp->left = std::move(y);
            xp->left->right = std::unique_ptr<Node<T, V>>(px);
            xp->left->right->parent = xp->left.get();
        } else {
            auto px = x.release();
            xp->right = std::move(y);
            xp->right->right = std::unique_ptr<Node<T, V>>(px);
            xp->right->right->parent = xp->right.get();
        }
    }

public:
    /*
        find
        Принимает: ключ
        ищет узел по ключу
        Возвращает: указатель на найденный узел
    */
    Node<T, V>* find(const T& key) {
        return Search(root.get(), key);
    }

    /*
        insert
        Принимает: (ключ, значение)
        вставляет узел (ключ, значение)
        Возвращает: void

    */
    void insert(std::pair<T, V> p) {
    //void insert(const T& key, const V& val) {
        auto z = std::make_unique<Node<T, V>>(p.first, p.second);
        Insert(std::move(z));
    }

    /*
        erase
        Принимает: ключ
        проводит удаление узла по ключу
        Возвращает: 0  если успешно
                    -1 если такого узла нет
    */
    int erase(const T& key) {
        auto z = Search(root.get(), key);
        return Delete(z);
    }

private:
    Node<T, V>* Search(Node<T, V>* x, const T& key) {
        if (!x || x->key == key) {
            return x;
        }
        if (key < x->key) {
            return Search(x->left.get(), key);
        } else {
            return Search(x->right.get(), key);
        }
    }

    void Insert(std::unique_ptr<Node<T, V>> z) {
        Node<T, V>* y = nullptr;
        Node<T, V>* x = root.get();
        while (x) {
            y = x;
            if (z->key < x->key) {
                x = x->left.get();
            } else {
                x = x->right.get();
            }
        }
        z->parent = y;
        if (!y) {
            root = std::move(z);
            InsertFixup(std::move(root));
        } else if (z->key < y->key) {
            y->left = std::move(z);
            InsertFixup(std::move(y->left));
        } else {
            y->right = std::move(z);
            InsertFixup(std::move(y->right));
        }
    }

    void InsertFixup(std::unique_ptr<Node<T, V>>&& z) {
        auto zp = z->parent;
        while (zp && zp->color == Color::Red) {
            auto zpp = zp->parent;
            if (zp == zpp->left.get()) {
                auto y = zpp->right.get();
                if (y && y->color == Color::Red) {
                    zp->color = Color::Black;
                    y->color = Color::Black;
                    zpp->color = Color::Red;
                    zp = zpp->parent;
                } else {
                    if (z == zp->right) {
                        LeftRotate(std::move(zpp->left));
                        zp = zpp->left.get();
                    }
                    zp->color = Color::Black;
                    zpp->color = Color::Red;
                    auto zppp = zpp->parent;
                    if (!zppp) {
                        RightRotate(std::move(root));
                    } else if (zpp == zppp->left.get()) {
                        RightRotate(std::move(zppp->left));
                    } else {
                        RightRotate(std::move(zppp->right));
                    }
                }
            } else {
                auto y = zpp->left.get();
                if (y && y->color == Color::Red) {
                    zp->color = Color::Black;
                    y->color = Color::Black;
                    zpp->color = Color::Red;
                    zp = zpp->parent;
                } else {
                    if (z == zp->left) {
                        RightRotate(std::move(zpp->right));
                        zp = zpp->right.get();
                    }
                    zp->color = Color::Black;
                    zpp->color = Color::Red;
                    auto zppp = zpp->parent;
                    if (!zppp) {
                        LeftRotate(std::move(root));
                    } else if (zpp == zppp->left.get()) {
                        LeftRotate(std::move(zppp->left));
                    } else {
                        LeftRotate(std::move(zppp->right));
                    }
                }
            }
        }
        root->color = Color::Black;
    }

    Node<T, V>* Transplant(Node<T, V>* u, std::unique_ptr<Node<T, V>>&& v) {
        if (v) {
            v->parent = u->parent;
        }
        Node<T, V>* w = nullptr;
        if (!u->parent) {
            w = root.release();
            root = std::move(v);
        } else if (u == u->parent->left.get()) {
            w = u->parent->left.release();
            u->parent->left = std::move(v);
        } else {
            w = u->parent->right.release();
            u->parent->right = std::move(v);
        }
        return w;
    }

    Node<T, V>* Minimum(Node<T, V>* x) {
        if (!x) {
            return x;
        }
        while (x->left) {
            x = x->left.get();
        }
        return x;
    }

    int Delete(Node<T, V>* z) {
        if (!z) {
            return -1;
        }
        Color orig_color = z->color;
        Node<T, V>* x = nullptr;
        Node<T, V>* xp = nullptr;
        if (!z->left) {
            x = z->right.get();
            xp = z->parent;
            auto pz = Transplant(z, std::move(z->right));
            auto upz = std::unique_ptr<Node<T, V>>(pz);
        } else if (!z->right) {
            x = z->left.get();
            xp = z->parent;
            auto pz = Transplant(z, std::move(z->left));
            auto upz = std::unique_ptr<Node<T, V>>(pz);
        } else {
            auto y = Minimum(z->right.get());
            orig_color = y->color;
            x = y->right.get();
            xp = y;
            if (y->parent == z) {
                if (x) {
                    x->parent = y;
                }
                auto pz = Transplant(z, std::move(z->right));
                y->left = std::move(pz->left);
                y->left->parent = y;
                y->color = pz->color;
                auto upz = std::unique_ptr<Node<T, V>>(pz);
            } else {
                xp = y->parent;
                auto py = Transplant(y, std::move(y->right));
                py->right = std::move(z->right);
                py->right->parent = py;
                auto upy = std::unique_ptr<Node<T, V>>(py);
                auto pz = Transplant(z, std::move(upy));
                py->left = std::move(pz->left);
                py->left->parent = py;
                py->color = pz->color;
                auto upz = std::unique_ptr<Node<T, V>>(pz);
            }
        }
        if (orig_color == Color::Black) {
            DeleteFixup(x, xp);
        }

        return 0;
    }

    void DeleteFixup(Node<T, V>* x, Node<T, V>* xp) {
        while (x != root.get() && (!x || x->color == Color::Black)) {
            if (x == xp->left.get()) {
                Node<T, V>* w = xp->right.get();
                if (w && w->color == Color::Red) {
                    w->color = Color::Black;
                    xp->color = Color::Red;
                    auto xpp = xp->parent;
                    if (!xpp) {
                        LeftRotate(std::move(root));
                    } else if (xp == xpp->left.get()) {
                        LeftRotate(std::move(xpp->left));
                    } else {
                        LeftRotate(std::move(xpp->right));
                    }
                    w = xp->right.get();
                }
                if (w && (!w->left || w->left->color == Color::Black)
                    && (!w->right || w->right->color == Color::Black)) {
                    w->color = Color::Red;
                    x = xp;
                    xp = xp->parent;
                } else if (w) {
                    if (!w->right || w->right->color == Color::Black) {
                        w->left->color = Color::Black;
                        w->color = Color::Red;
                        auto wp = w->parent;
                        if (!wp) {
                            RightRotate(std::move(root));
                        } else if (w == wp->left.get()) {
                            RightRotate(std::move(wp->left));
                        } else {
                            RightRotate(std::move(wp->right));
                        }
                        w = xp->right.get();
                    }
                    w->color = xp->color;
                    xp->color = Color::Black;
                    w->right->color = Color::Black;
                    auto xpp = xp->parent;
                    if (!xpp) {
                        LeftRotate(std::move(root));
                    } else if (xp == xpp->left.get()) {
                        LeftRotate(std::move(xpp->left));
                    } else {
                        LeftRotate(std::move(xpp->right));
                    }
                    x = root.get();
                } else {
                    x = root.get();
                }
            } else {
                Node<T, V>* w = xp->left.get();
                if (w && w->color == Color::Red) {
                    w->color = Color::Black;
                    xp->color = Color::Red;
                    auto xpp = xp->parent;
                    if (!xpp) {
                        RightRotate(std::move(root));
                    } else if (xp == xpp->left.get()) {
                        RightRotate(std::move(xpp->left));
                    } else {
                        RightRotate(std::move(xpp->right));
                    }
                    w = xp->left.get();
                }
                if (w && (!w->left || w->left->color == Color::Black)
                    && (!w->right || w->right->color == Color::Black)) {
                    w->color = Color::Red;
                    x = xp;
                    xp = xp->parent;
                } else if (w) {
                    if (!w->left || w->left->color == Color::Black) {
                        w->right->color = Color::Black;
                        w->color = Color::Red;
                        auto wp = w->parent;
                        if (!wp) {
                            LeftRotate(std::move(root));
                        } else if (w == wp->left.get()) {
                            LeftRotate(std::move(wp->left));
                        } else {
                            LeftRotate(std::move(wp->right));
                        }
                        w = xp->left.get();
                    }
                    w->color = xp->color;
                    xp->color = Color::Black;
                    w->left->color = Color::Black;
                    auto xpp = xp->parent;
                    if (!xpp) {
                        RightRotate(std::move(root));
                    } else if (xp == xpp->left.get()) {
                        RightRotate(std::move(xpp->left));
                    } else {
                        RightRotate(std::move(xpp->right));
                    }
                    x = root.get();
                } else {
                    x = root.get();
                }
            }
        }
        if (x) {
            x->color = Color::Black;
        }
    }

};

// Можно распечатать узел с его потомками в инфиксном порядке
template <typename T, typename V>
std::ostream& operator<<(std::ostream& os, Node<T, V>* node) {
    if (node) {
        os << node->left.get();
        os << ' ' << node->key;
        /*
        if (node->color == Color::Black) {
            os << "* "; // черное
        } else {
            os << "o "; // красное
        }
        */
        os << node->right.get();
    }
    return os;
}

// Можно распечатать дерево целиком в инфиксном порядке
template <typename T, typename V>
std::ostream& operator<<(std::ostream& os, const RBTree<T, V>& tree) {
    os << tree.root.get();
    return os;
}


// Пример работы
/*
int main() {

    RBTree<int, int> tree;

    for (int i=0; i<25; i++)
        tree.insert({i, i});

    std::cout << tree << '\n';

    std::cout << (tree.find(10))->value << '\n';

    for (int i=0; i<25; i++)
        tree.erase(i);



    return 0;
}
*/
