#ifndef AVL_TREE_ITERATOR_H
#define AVL_TREE_ITERATOR_H

#include <initializer_list>

template<typename KeyType, typename ValueType>
class Avl_tree;


template<typename KeyType, typename ValueType, typename PtrType>
class Avl_tree_iterator: public std::iterator<std::input_iterator_tag, PtrType>
{
    friend class Avl_tree<KeyType,ValueType>;
private:
    Avl_tree_iterator(PtrType* p) : p(p) {}
	Avl_tree_iterator& find_min(Avl_tree_iterator& p) const { return p->left ? find_min(p->left) : p; }	// search for the minimum key of a given node

public:
    Avl_tree_iterator(const Avl_tree_iterator &it) : p(it.p) {}
    bool operator!=(Avl_tree_iterator const& other) const { return p != other.p; }
    bool operator==(Avl_tree_iterator const& other) const { return p == other.p; } //need for BOOST_FOREACH
    typename Avl_tree_iterator::reference operator*() const { return *p; }
private:
    PtrType* p;
};

#endif
