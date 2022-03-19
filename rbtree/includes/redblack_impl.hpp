#ifndef RBTREE_INCLUDES_REDBLACK_IMPL_HPP
#define RBTREE_INCLUDES_REDBLACK_IMPL_HPP

#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

#include "../../Stack/Stack/includes/Stack.h"
#include "../../Stack/Stack/includes/Stack_impl.h"
#include "redblack.hpp"

#define RED true
#define BLACK false

template <class T>
NodeT<T>::NodeT() {
  color_ = RED;
  frequency_ = 0;

  left_ = nullptr;
  right_ = nullptr;
  parent_ = nullptr;
}

template <class T>
NodeT<T>::NodeT(const NodeT<T>& other) {
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

  left_ = nullptr;
  right_ = nullptr;
  parent_ = nullptr;
}

template <class T>
NodeT<T>::NodeT(NodeT<T>&& other) noexcept {
  data_ = other.data_;
  color_ = other.color_;
  frequency_ = other.frequency_;
  left_ = other.left_;
  right_ = other.right_;
  parent_ = other.parent_;

  other.left_ = nullptr;
  other.right_ = nullptr;
  other.parent_ = nullptr;
}

template <class T>
NodeT<T>* RBTree<T>::grandparent(NodeT<T>* ptr) {
  if ((ptr != nullptr) && (ptr->parent_ != nullptr) && (ptr->parent_->parent_ != nullptr))
    return ptr->parent_->parent_;

  return nullptr;
}

template <class T>
NodeT<T>* RBTree<T>::uncle(NodeT<T>* ptr) {
  if (grandparent(ptr) == nullptr)
    return nullptr;

  if ((grandparent(ptr)->left_ == ptr->parent_) && (grandparent(ptr)->right_ != nullptr))
    return grandparent(ptr)->right_;

  if ((grandparent(ptr)->right_ == ptr->parent_) && (grandparent(ptr)->left_ != nullptr))
    return grandparent(ptr)->left_;

  return nullptr;
}

template <class T>
NodeT<T>* RBTree<T>::sibling(NodeT<T>* ptr) {
  if (ptr->parent_ == nullptr)
    return nullptr;

  if (ptr == ptr->parent_->left_)
    return ptr->parent_->right_;

  return ptr->parent_->left_;
}

template <class T>
RBTree<T>::RBTree() {
  root_ = nullptr;
  size_ = 0;
}

template <class T>
RBTree<T>::RBTree(const RBTree<T>& other) {
  size_ = other.size_;
  root_ = copy_tree(other.root_);
}

template <class T>
RBTree<T>::RBTree(RBTree<T>&& other) noexcept {
  size_ = other.size_;
  root_ = other.root_;

  other.root_ = nullptr;
}

template <class T>
void RBTree<T>::delete_tree(NodeT<T>* ptr) {
  if (ptr == nullptr)
    return;

  assert(ptr);

  if (ptr->left_ != nullptr)
    delete_tree(ptr->left_);

  if (ptr->right_ != nullptr)
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
  if (n == nullptr)
    return;

  NodeT<T>* pivot = n->right_;

  pivot->parent_ = n->parent_;
  if (pivot->parent_ == nullptr && root_ == n) {
    root_ = pivot;
  }

  if (n->parent_ != nullptr) {
    if (n->parent_->left_ == n)
      n->parent_->left_ = pivot;

    else
      n->parent_->right_ = pivot;
  }

  n->right_ = pivot->left_;

  if (pivot->left_ != nullptr)
    pivot->left_->parent_ = n;

  n->parent_ = pivot;
  pivot->left_ = n;
}

template <class T>
void RBTree<T>::rotate_right(NodeT<T>* n) {
  if (n == nullptr)
    return;

  NodeT<T>* pivot = n->left_;

  pivot->parent_ = n->parent_;
  if (pivot->parent_ == nullptr && root_ == n) {
    root_ = pivot;
  }

  if (n->parent_ != nullptr) {
    if (n->parent_->left_ == n)
      n->parent_->left_ = pivot;

    else
      n->parent_->right_ = pivot;
  }

  n->left_ = pivot->right_;

  if (pivot->right_ != nullptr)
    pivot->right_->parent_ = n;

  n->parent_ = pivot;
  pivot->right_ = n;
}

template <class T>
NodeT<T>* RBTree<T>::insert(const T& rhs) {
  auto cur = new NodeT<T>();

  cur->frequency_ = 1;
  cur->data_ = rhs;
  cur->color_ = RED;
  cur->left_ = cur->right_ = nullptr;

  if (!size_) {
    ++size_;
    root_ = cur;
    root_->color_ = BLACK;
    root_->parent_ = nullptr;
    root_->left_ = nullptr;
    root_->right_ = nullptr;
    return nullptr;
  }

  NodeT<T>* mind = root_;

  while (true) {
    if (mind->data_ == rhs) {
      ++mind->frequency_;
      delete cur;
      return nullptr;
    }

    if (mind->data_ > rhs) {
      if (mind->left_ != nullptr)
        mind = mind->left_;

      else {
        ++size_;
        mind->left_ = cur;
        cur->parent_ = mind;
        return cur;
      }
    }

    if (mind->data_ < rhs) {
      if (mind->right_ != nullptr)
        mind = mind->right_;

      else {
        ++size_;
        mind->right_ = cur;
        cur->parent_ = mind;
        return cur;
      }
    }
  }
}

template <class T>
void RBTree<T>::balance(NodeT<T>* ptr) {
  if (ptr == nullptr)
    return;

  insert_case1(ptr);
}

template <class T>
void RBTree<T>::insert_case1(NodeT<T>* ptr) {
  if (ptr->parent_ == nullptr)
    ptr->color_ = BLACK;

  else
    insert_case2(ptr);
}

template <class T>
void RBTree<T>::insert_case2(NodeT<T>* ptr) {
  if (ptr->parent_->color_ == BLACK)
    return;

  else if (grandparent(ptr) != nullptr)
    insert_case3(ptr);
}

template <class T>
void RBTree<T>::insert_case3(NodeT<T>* ptr) {
  NodeT<T>* u = uncle(ptr);
  NodeT<T>* g;

  if ((u != nullptr) && (u->color_ == RED)) {
    ptr->parent_->color_ = BLACK;
    u->color_ = BLACK;
    g = grandparent(ptr);
    g->color_ = RED;
    insert_case1(g);
  } else {
    insert_case4(ptr);
  }
}

template <class T>
void RBTree<T>::insert_case4(NodeT<T>* ptr) {
  NodeT<T>* g = grandparent(ptr);

  if ((ptr == ptr->parent_->right_) && (ptr->parent_ == g->left_)) {
    rotate_left(ptr->parent_);
    ptr = ptr->left_;
  } else if ((ptr == ptr->parent_->left_) && (ptr->parent_ == g->right_)) {
    rotate_right(ptr->parent_);
    ptr = ptr->right_;
  }

  insert_case5(ptr);
}

template <class T>
void RBTree<T>::insert_case5(NodeT<T>* ptr) {
  NodeT<T>* g = nullptr;
  g = grandparent(ptr);

  ptr->parent_->color_ = BLACK;
  g->color_ = RED;

  if ((ptr == ptr->parent_->left_) && (ptr->parent_ == g->left_)) {
    rotate_right(g);
  } else if ((ptr == ptr->parent_->right_) && (ptr->parent_ == g->right_)) {
    rotate_left(g);
  }
}

template <class T>
void RBTree<T>::add(const T& rhs) {
  NodeT<T>* res = insert(rhs);

  balance(res);
}

template <class T>
bool RBTree<T>::search(const T& rhs) {
  auto res = inorder();

  for (auto n : res) {
    if (n->data_ == rhs) {
      return true;
    }
  }

  return false;
}

template <class T>
NodeT<T>* RBTree<T>::copy_tree(const NodeT<T>* other) {
  auto res = new NodeT<T>();
  res->color_ = other->color_;
  res->data_ = other->data_;
  res->parent_ = other->parent_;

  if (other->left_ != nullptr)
    res->left_ = copy_tree(other->left_);

  if (other->right_ != nullptr)
    res->right_ = copy_tree(other->right_);

  return res;
}

template <class T>
RBTree<T>& RBTree<T>::operator=(const RBTree<T>& other) {
  if (this != &other) {
    delete_tree(root_);

    size_ = other.size_;
    root_ = copy_tree(other.root_);
  }

  return *this;
}

template <class T>
RBTree<T>& RBTree<T>::operator=(RBTree<T>&& other) noexcept {
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
  if (this == &other)
    return true;

  if (size_ != other.size_)
    return false;

  return checkequality(root_, other.root_);
}

template <class T>
bool RBTree<T>::checkequality(const NodeT<T>* froot, const NodeT<T>* sroot) const {
  if (froot == nullptr || sroot == nullptr)
    return (froot == nullptr && sroot == nullptr);

  if (froot->frequency_ != sroot->frequency_)
    return false;

  if (froot->data_ != sroot->data_)
    return false;

  if (froot->color_ != sroot->color_)
    return false;

  return (checkequality(froot->left_, sroot->left_) && checkequality(froot->right_, sroot->right_));
}

template <class T>
bool RBTree<T>::operator!=(const RBTree<T>& other) const {
  return !(*this == other);
}

template <class T>
std::vector<NodeT<T>*> RBTree<T>::inorder() const {
  Stack<NodeT<T>*> s;
  auto cur = root_;
  std::vector<NodeT<T>*> resarr;

  while (true) {
    while (cur != nullptr) {
      s.push(cur);
      cur = cur->left_;
    }

    if (cur == nullptr && !s.is_empty()) {
      auto mind = s.top();
      resarr.push_back(mind);
      cur = mind->right_;
      s.pop();
    }

    if (cur == nullptr && s.is_empty()) {
      break;
    }
  }

  return resarr;
}

template <class T>
void RBTree<T>::print_inorder() const {
  auto res = inorder();
  for (auto n : res) {
    std::cout << "data " << n->data_;
    std::cout << " freq " << n->frequency_ << std::endl;
  }
}

template <class T>
T RBTree<T>::find_most_frequent() const {
  auto res = inorder();
  size_t fr = 0;
  T ans;

  for (auto n : res) {
    if (n->frequency_ > fr) {
      fr = n->frequency_;
      ans = n->data_;
    }
  }

  return ans;
}

#endif  // RBTREE_INCLUDES_REDBLACK_IMPL_HPP