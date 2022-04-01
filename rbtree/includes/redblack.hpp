#ifndef RBTREE_INCLUDES_REDBLACK_HPP
#define RBTREE_INCLUDES_REDBLACK_HPP

#include <cstdlib>
#include <vector>

template <class T>
struct NodeT {
  NodeT();
  NodeT(const NodeT<T>& other);
  explicit NodeT(const T& rhs);
  NodeT(NodeT<T>&& other) noexcept;

  bool color_;
  T data_;
  size_t frequency_;

  NodeT<T>* left_;
  NodeT<T>* right_;
  NodeT<T>* parent_;
};

template <class T>
class RBTree {
 public:
  RBTree();
  RBTree(const RBTree<T>& other);
  RBTree(RBTree<T>&& other) noexcept;
  ~RBTree();

  bool is_empty() const;
  size_t getsize() const;
  std::pair<T, size_t> get_root() const;

  void add(const T& rhs);
  size_t delete_elem(const T& rhs);

  std::vector<T> get_inorder() const;
  void print_inorder() const;

  bool iselem(const T& rhs) const;
  size_t get_frequency(const T& val) const;
  void set_frequency(const T& val, const T& newfreq);

  RBTree<T>& operator=(const RBTree<T>& other);
  RBTree<T>& operator=(RBTree<T>&& other) noexcept;

  bool operator==(const RBTree<T>& other) const;
  bool operator!=(const RBTree<T>& other) const;

  T find_most_frequent() const;

 private:
  size_t size_;
  NodeT<T>* root_;

  NodeT<T>* search(const T& val) const;
  std::vector<NodeT<T>*> inorder() const;
  void delete_tree(NodeT<T>* ptr);
  NodeT<T>* copy_tree(const NodeT<T>* other);
  bool checkequality(const NodeT<T>* froot, const NodeT<T>* sroot) const;

  NodeT<T>* uncle(NodeT<T>* ptr);
  NodeT<T>* sibling(NodeT<T>* ptr);
  NodeT<T>* grandparent(NodeT<T>* ptr);

  void rotate_left(NodeT<T>* n);
  void rotate_right(NodeT<T>* n);

  NodeT<T>* insert(const T& rhs);
  void insert_case1(NodeT<T>* ptr);
  void insert_case2(NodeT<T>* ptr);
  void insert_case3(NodeT<T>* ptr);
  void insert_case4(NodeT<T>* ptr);
  void insert_case5(NodeT<T>* ptr);
  void balance(NodeT<T>* ptr);

  NodeT<T>* is_leaf(NodeT<T>* ptr);
  NodeT<T>* find_desc(NodeT<T>* ptr);
  void delete_one_child(NodeT<T>* ptr);
  void replace_node(NodeT<T>* first, NodeT<T>* second);

  void delete_case1(NodeT<T>* ptr);
  void delete_case2(NodeT<T>* ptr);
  void delete_case3(NodeT<T>* ptr);
  void delete_case4(NodeT<T>* ptr);
  void delete_case5(NodeT<T>* ptr);
  void delete_case6(NodeT<T>* ptr);
};

#endif  // RBTREE_INCLUDES_REDBLACK_HPP