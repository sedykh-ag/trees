#ifndef AVL_TREE_ITERATOR_H
#define AVL_TREE_ITERATOR_H

#include <initializer_list>
#include"Avl_tree.h"

template<typename KeyType, typename ValueType>
class Avl_tree;


template<typename KeyType, typename ValueType, typename PtrType>
class Avl_tree_iterator: public std::iterator<std::input_iterator_tag, PtrType>
{
    friend class Avl_tree<KeyType,ValueType>;
private:
    Avl_tree_iterator(PtrType p, Avl_tree<KeyType,ValueType>* t) : p(p), tr(t) {}

public:
    Avl_tree_iterator(const Avl_tree_iterator &it) : p(it.p) {}
    bool operator!=(Avl_tree_iterator const& other) const { return p != other.p; }
    bool operator==(Avl_tree_iterator const& other) const { return p == other.p; } //need for BOOST_FOREACH
    typename Avl_tree_iterator::reference operator*() const { return *p; }
    typename Avl_tree_iterator::reference operator->() { return p; }
    Avl_tree_iterator& operator++() {
		p = next_node(p);
		return *this;
    }

private:
    PtrType p;
    Avl_tree<KeyType,ValueType>* tr;
    PtrType next_node (PtrType p) {
		if (p == end()) return p;
        if (p->right != nullptr) {
            return find_min(p->right);
        }
		if (p->parent.lock() != nullptr) {
			if (p == ((p->parent).lock())->left) {
				return (p->parent).lock();
			}
			else {
				if (p->right == nullptr) {
                    return  (((p->parent).lock())->parent).lock();
                }
				else return find_min(p->right);
			}
		}
		else {
			return find_min(p->right);
		}
    }
	PtrType find_min(PtrType p) const { return p->left ? find_min(p->left) : p; }
	PtrType find_max(PtrType p) const { return p->right ? find_max(p->right) : p; }
	PtrType end() { return find_max((*tr).root); }

};

#endif
