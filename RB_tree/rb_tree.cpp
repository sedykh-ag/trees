#include <bits/stdc++.h>
#include "node.cpp"

// для работы непосредственно с указателями на узлы
// удобнее функции пихать не в методы узлов
// а использовать их как дружественные функции
// получить родителя nullptr методом класса -- проблема

template<class Key, class T>
class RBTree {
private:
    /* VARS */
    Node<Key, T>* root;

    /* FUNCTIONS */

    // Helper functions:
    Node<Key, T>* GetParent(Node<Key, T>* n) {
      // Note that parent is set to null for the root node.
      return n == nullptr ? nullptr : n->parent;
    }

    Node<Key, T>* GetGrandParent(Node<Key, T>* n) {
      // Note that it will return nullptr if this is root or child of root
      return GetParent(GetParent(n));
    }

    Node<Key, T>* GetSibling(Node<Key, T>* n) {
      Node<Key, T>* p = GetParent(n);

      // No parent means no sibling.
      if (p == nullptr) {
        return nullptr;
      }

      if (n == p->left) {
        return p->right;
      } else {
        return p->left;
      }
    }

    Node<Key, T>* GetUncle(Node<Key, T>* n) {
      Node<Key, T>* p = GetParent(n);

      // No parent means no uncle
      return GetSibling(p);
    }

    // Rotations
    void RotateLeft(Node<Key, T>* n) {
      Node<Key, T>* nnew = n->right;
      Node<Key, T>* p = GetParent(n);
      assert(nnew != nullptr);  // Since the leaves of a red-black tree are empty,
                                // they cannot become internal nodes.
      n->right = nnew->left;
      nnew->left = n;
      n->parent = nnew;
      // Handle other child/parent pointers.
      if (n->right != nullptr) {
        n->right->parent = n;
      }

      // Initially n could be the root.
      if (p != nullptr) {
        if (n == p->left) {
          p->left = nnew;
        } else if (n == p->right) {
          p->right = nnew;
        }
      }
      nnew->parent = p;
    }

    void RotateRight(Node<Key, T>* n) {
      Node<Key, T>* nnew = n->left;
      Node<Key, T>* p = GetParent(n);
      assert(nnew != nullptr);  // Since the leaves of a red-black tree are empty,
                                // they cannot become internal nodes.

      n->left = nnew->right;
      nnew->right = n;
      n->parent = nnew;

      // Handle other child/parent pointers.
      if (n->left != nullptr) {
        n->left->parent = n;
      }

      // Initially n could be the root.
      if (p != nullptr) {
        if (n == p->left) {
          p->left = nnew;
        } else if (n == p->right) {
          p->right = nnew;
        }
      }
      nnew->parent = p;
    }

    // Insertions
    Node<Key, T>* Insert(Node<Key, T>* root, Node<Key, T>* n) {
      // Insert new Node<Key, T> into the current tree.
      InsertRecurse(root, n);

      // Repair the tree in case any of the red-black properties have been violated.
      InsertRepairTree(n);

      // Find the new root to return.
      root = n;
      while (GetParent(root) != nullptr) {
        root = GetParent(root);
      }
      return root;
    }

    void InsertRecurse(Node<Key, T>* root, Node<Key, T>* n) {
      // Recursively descend the tree until a leaf is found.
      if (root != nullptr)
      {
        if (n->key < root->key) {
          if (root->left != nullptr) {
            InsertRecurse(root->left, n);
            return;
          } else {
            root->left = n;
          }
        } else { // n->key >= root->key
          if (root->right != nullptr) {
            InsertRecurse(root->right, n);
            return;
          } else {
            root->right = n;
          }
        }
      }

      // Insert new Node<Key, T> n.
      n->parent = root;
      n->left = nullptr;
      n->right = nullptr;
      n->color = RED;
    }

    void InsertRepairTree(Node<Key, T>* n) {
      if (GetParent(n) == nullptr) {
        InsertCase1(n);
      } else if (GetParent(n)->color == BLACK) {
        InsertCase2(n);
      } else if (GetUncle(n) != nullptr && GetUncle(n)->color == RED) {
        InsertCase3(n);
      } else {
        InsertCase4(n);
      }
    }

    // INSERTION cases

    // Case 1
    void InsertCase1(Node<Key, T>* n) {
      n->color = BLACK;
    }

    // Case 2
    void InsertCase2(Node<Key, T>* n) {
      // Do nothing since tree is still valid.
      return;
    }

    //Case 3
    void InsertCase3(Node<Key, T>* n) {
      GetParent(n)->color = BLACK;
      GetUncle(n)->color = BLACK;
      GetGrandParent(n)->color = RED;
      InsertRepairTree(GetGrandParent(n));
    }

    //Case 4 step 1
    void InsertCase4(Node<Key, T>* n) {
      Node<Key, T>* p = GetParent(n);
      Node<Key, T>* g = GetGrandParent(n);

      if (n == p->right && p == g->left) {
        RotateLeft(p);
        n = n->left;
      } else if (n == p->left && p == g->right) {
        RotateRight(p);
        n = n->right;
      }

      InsertCase4Step2(n);
    }

    //Case 4 step 2
    void InsertCase4Step2(Node<Key, T>* n) {
      Node<Key, T>* p = GetParent(n);
      Node<Key, T>* g = GetGrandParent(n);

      if (n == p->left) {
        RotateRight(g);
      } else {
        RotateLeft(g);
      }
      p->color = BLACK;
      g->color = RED;
    }

    // REMOVAL
    void ReplaceNode(Node<Key, T>* n, Node<Key, T>* child) {
      child->parent = n->parent;
      if (n == n->parent->left) {
        n->parent->left = child;
      } else {
        n->parent->right = child;
      }
    }

    void DeleteOneChild(Node<Key, T>* n) {
      // Precondition: n has at most one non-leaf child.
      Node<Key, T>* child = (n->right == nullptr) ? n->left : n->right;
      assert(child);

      ReplaceNode(n, child);
      if (n->color == BLACK) {
        if (child->color == RED) {
          child->color = BLACK;
        } else {
          DeleteCase1(child);
        }
      }
      free(n);
    }

    // REMOVAL cases

    // Case 1
    void DeleteCase1(Node<Key, T>* n) {
      if (n->parent != nullptr) {
        DeleteCase2(n);
      }
    }

    // Case 2
    void DeleteCase2(Node<Key, T>* n) {
      Node<Key, T>* s = GetSibling(n);

      if (s->color == RED) {
        n->parent->color = RED;
        s->color = BLACK;
        if (n == n->parent->left) {
          RotateLeft(n->parent);
        } else {
          RotateRight(n->parent);
        }
      }
      DeleteCase3(n);
    }

    // Case 3
    void DeleteCase3(Node<Key, T>* n) {
      Node<Key, T>* s = GetSibling(n);

      if ((n->parent->color == BLACK) && (s->color == BLACK) &&
          (s->left->color == BLACK) && (s->right->color == BLACK)) {
        s->color = RED;
        DeleteCase1(n->parent);
      } else {
        DeleteCase4(n);
      }
    }

    // Case 4
    void DeleteCase4(Node<Key, T>* n) {
      Node<Key, T>* s = GetSibling(n);

      if ((n->parent->color == RED) && (s->color == BLACK) &&
          (s->left->color == BLACK) && (s->right->color == BLACK)) {
        s->color = RED;
        n->parent->color = BLACK;
      } else {
        DeleteCase5(n);
      }
    }

    // Case 5
    void DeleteCase5(Node<Key, T>* n) {
      Node<Key, T>* s = GetSibling(n);

      // This if statement is trivial, due to case 2 (even though case 2 changed
      // the sibling to a sibling's child, the sibling's child can't be red, since
      // no red parent can have a red child).
      if (s->color == BLACK) {
        // The following statements just force the red to be on the left of the
        // left of the parent, or right of the right, so case six will rotate
        // correctly.
        if ((n == n->parent->left) && (s->right->color == BLACK) &&
            (s->left->color == RED)) {
          // This last test is trivial too due to cases 2-4.
          s->color = RED;
          s->left->color = BLACK;
          RotateRight(s);
        } else if ((n == n->parent->right) && (s->left->color == BLACK) &&
                   (s->right->color == RED)) {
          // This last test is trivial too due to cases 2-4.
          s->color = RED;
          s->right->color = BLACK;
          RotateLeft(s);
        }
      }
      DeleteCase6(n);
    }

    // Case 6
    void DeleteCase6(Node<Key, T>* n) {
      Node<Key, T>* s = GetSibling(n);

      s->color = n->parent->color;
      n->parent->color = BLACK;

      if (n == n->parent->left) {
        s->right->color = BLACK;
        RotateLeft(n->parent);
      } else {
        s->left->color = BLACK;
        RotateRight(n->parent);
      }
    }


public:
    // Member types
    using value_type = std::pair<Key, T>;
    class iterator { }; // FIXME

    // FINDING

    // INSERTION
    std::pair<iterator, bool> insert( value_type& value ) {
        std::shared_ptr<Node<Key, T>> n = new Node<Key, T>(value.first, value.second);
        root = Insert(root, n);
    }

    // REMOVAL
    int erase (Key& key) {
        // need to use finding first
    }




};

int main() {

    return 0;
}
