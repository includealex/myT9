#ifndef RBTREE_INCLUDES_REDBLACK_IMPL_HPP
#define RBTREE_INCLUDES_REDBLACK_IMPL_HPP

#include <algorithm>
#include <iostream>

#include "redblack.hpp"

#define RED true
#define BLACK false

template <class T>
NodeT<T>::NodeT() {
  color_ = RED;
  frequency_ = 0;
  left_ = right_ = parent_ = nullptr;
}

template <class T>
NodeT<T>::NodeT(const NodeT& other) {
  data_ = other.data_;
  color_ = other.color_;
  frequency_ = other.frequency_;

  left_ = other.left_;
  right_ = other.right_;
  parent_ = other.parent_;
}

template <class T>
NodeT<T>::NodeT(const T& rhs) {
  data_ = rhs;
  color_ = BLACK;
  frequency_ = 1;

  left_ = right_ = parent_ = nullptr;
}

template <class T>
NodeT<T>::NodeT(NodeT&& other) noexcept {
  std::swap(data_, other.data_);
  std::swap(color_, other.color_);
  std::swap(frequency_, other.frequency_);

  std::swap(left_, other.left_);
  std::swap(right_, other.right_);
  std::swap(parent_, other.parent_);
}

template <class T>
NodeT<T>* RBTree<T>::grandparent(NodeT<T>* ptr) {
  if (ptr != nullptr && ptr->parent_ && ptr->parent_->parent_)
    return ptr->parent_->parent_;

  return nullptr;
}

template <class T>
NodeT<T>* RBTree<T>::uncle(NodeT<T>* ptr) {
  if (!grandparent(ptr))
    return nullptr;

  if (grandparent(ptr)->left_ == ptr->parent_)
    return grandparent(ptr)->right_;

  return grandparent(ptr)->left_;
}

template <class T>
RBTree<T>::RBTree() {
  size_ = 0;
}

template <class T>
RBTree<T>::RBTree(const RBTree<T>& other) {
  size_ = other.size_;
  root_ = other.root_;
}

template <class T>
RBTree<T>::RBTree(RBTree<T>&& other) noexcept {
  std::swap(size_, other.size_);
  std::swap(root_, other.root_);
}

template <class T>
void RBTree<T>::delete_tree(NodeT<T>* ptr) {
  if (!ptr)
    return;

  delete_tree(ptr->left_);
  delete_tree(ptr->right_);
  delete ptr;
}

template <class T>
RBTree<T>::~RBTree() {
  delete_tree(root_);
}

template <class T>
bool RBTree<T>::is_empty() const {
  return (size_ == 0);
}

template <class T>
size_t RBTree<T>::getsize() const {
  return size_;
}

template <class T>
NodeT<T>* RBTree<T>::get_root() const {
  return root_;
}

template <class T>
void RBTree<T>::rotate_left(NodeT<T>* n) {
  if (!n) {
    return;
  }

  if (n == root_)
    return;

  NodeT<T>* pivot = n->right_;

  pivot->parent_ = n->parent_;

  if (n->parent_) {
    if (n->parent_->left_ == n)
      n->parent_->left_ = pivot;

    else
      n->parent_->right_ = pivot;
  }

  n->right_ = pivot->left_;

  if (pivot->left_)
    pivot->left_->parent_ = n;

  n->parent_ = pivot;
  pivot->left_ = n;
}

template <class T>
void RBTree<T>::rotate_right(NodeT<T>* n) {
  if (!n) {
    return;
  }

  if (n == root_)
    return;

  if (!n->left_)
    return;

  NodeT<T>* pivot = n->left_;

  pivot->parent_ = n->parent_;

  if (n->parent_) {
    if (n->parent_->left_ == n)
      n->parent_->left_ = pivot;

    else
      n->parent_->right_ = pivot;
  }

  n->left_ = pivot->right_;

  if (pivot->right_)
    pivot->right_->parent_ = n;

  n->parent_ = pivot;
  pivot->right_ = n;
}

template <class T>
NodeT<T>* RBTree<T>::insert(const T& rhs) {
  auto cur = new NodeT<T>;

  cur->data_ = rhs;
  cur->color_ = RED;
  cur->left_ = cur->right_ = nullptr;

  if (!size_) {
    root_ = cur;
    cur->parent_ = nullptr;
    ++size_;
    return cur;
  }

  if (root_->data_ == rhs) {
    ++root_->frequency_;
    delete cur;
    return nullptr;
  }

  NodeT<T>* mind = root_;

  while (true) {
    if (mind->data_ == rhs) {
      ++mind->frequency_;
      delete cur;
      return nullptr;
    }

    if (mind->data_ < rhs) {
      if (mind->right_) {
        mind = mind->right_;
      } else {
        mind->right_ = cur;
        cur->parent_ = mind;
        ++size_;
        return cur;
      }
    }

    else if (mind->data_ > rhs) {
      if (mind->left_) {
        mind = mind->left_;
      }

      else {
        mind->left_ = cur;
        cur->parent_ = mind;
        ++size_;
        return cur;
      }
    }
  }
}

template <class T>
void RBTree<T>::balance(NodeT<T>* ptr) {
  insert_case1(ptr);
}

template <class T>
void RBTree<T>::insert_case1(NodeT<T>* ptr) {
  if (!ptr) {
    return;
  }

  if (!ptr->parent_) {
    ptr->color_ = BLACK;
  }

  else {
    insert_case2(ptr);
  }
}

template <class T>
void RBTree<T>::insert_case2(NodeT<T>* ptr) {
  if (!ptr)
    return;

  if (ptr->parent_->color_ == BLACK) {
    return;
  }

  insert_case3(ptr);
}

template <class T>
void RBTree<T>::insert_case3(NodeT<T>* ptr) {
  if (!ptr)
    return;

  NodeT<T>* u = uncle(ptr);
  NodeT<T>* g;

  if ((u) && (u->color_ == RED)) {
    ptr->parent_->color_ = BLACK;
    u->color_ = BLACK;
    g = grandparent(ptr);
    g->color_ = RED;
    insert_case1(g);
  }

  else {
    insert_case4(ptr);
  }

  return;
}

template <class T>
void RBTree<T>::insert_case4(NodeT<T>* ptr) {
  if (!ptr)
    return;

  if (grandparent(ptr)) {
    NodeT<T>* g = grandparent(ptr);

    if ((ptr == ptr->parent_->right_) && (ptr->parent_ == g->left_)) {
      rotate_left(ptr->parent_);

      ptr = ptr->left_;
    }

    else if ((ptr == ptr->parent_->left_) && (ptr->parent_ == g->right_)) {
      rotate_right(ptr->parent_);

      ptr = ptr->right_;
    }

    insert_case5(ptr);
  }

  return;
}

template <class T>
void RBTree<T>::insert_case5(NodeT<T>* ptr) {
  if (!ptr) {
    return;
  }

  NodeT<T>* g = grandparent(ptr);

  ptr->parent_->color_ = BLACK;
  g->color_ = RED;

  if ((ptr = ptr->parent_->left_) && (ptr->parent_ == g->left_)) {
    rotate_right(g);
  } else {
    rotate_left(g);
  }

  return;
}

template <class T>
void RBTree<T>::add(const T& rhs) {
  NodeT<T>* res = insert(rhs);

  if (res) {
    balance(res);
  }
}

template <class T>
bool RBTree<T>::search(const T& rhs) {
  NodeT<T>* cur = root_;

  while (1) {
    if (cur->data_ == rhs) {
      return true;
    }

    if (rhs > cur->data_) {
      if (cur->right_)
        cur = cur->right_;

      else
        return false;
    }

    if (rhs < cur->data_) {
      if (cur->left_)
        cur = cur->left_;

      else
        return false;
    }
  }

  return false;
}

template <class T>
NodeT<T>* RBTree<T>::copy_tree(const NodeT<T>& other, const NodeT<T>* parent) {
  auto res = new NodeT<T>;
  res->color_ = other.color_;
  res->data_ = other.data_;
  res->parent_ = parent;

  res->left_ = copy_tree(other->left_, res);
  res->right_ = copy_tree(other->right_, res);
  return res;
}

template <class T>
RBTree<T>& RBTree<T>::operator=(const RBTree& other) {
  if (this != &other) {
    delete_tree(root_);

    size_ = other.size_;
    root_ = copy_tree(other.root_, other.root_->parent_);
  }

  return *this;
}

template <class T>
RBTree<T>& RBTree<T>::operator=(const RBTree&& other) {
  if (this != &other) {
    delete_tree(root_);
    size_ = other.size_;
    root_ = other.root_;

    other.root_ = nullptr;
    other.size_ = 0;
  }

  return *this;
}

template <class T>
bool RBTree<T>::operator==(const RBTree<T>& other) const {
  if (size_ != other.size_)
    return false;

  if (this == &other)
    return true;

  return checkequality(root_, other.root_);
}

template <class T>
bool RBTree<T>::checkequality(const NodeT<T>* froot, const NodeT<T>* sroot) const {
  if(froot == nullptr || sroot == nullptr)
    return (froot == nullptr && sroot == nullptr);

  if(froot->data_ != sroot->data_)
    return false;

  if(froot->color_ != sroot->color_)
    return false;

  checkequality(froot->left_, sroot->left_);
  checkequality(froot->right_, sroot->right_);

  return true;  
}

template <class T>
bool RBTree<T>::operator!=(const RBTree<T>& other) const {
  return !(*this == other);
}

#endif  // RBTREE_INCLUDES_REDBLACK_IMPL_HPP