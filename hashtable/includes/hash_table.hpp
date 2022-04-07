#ifndef HASHTABLE_INCLUDES_HASH_TABLE_HPP
#define HASHTABLE_INCLUDES_HASH_TABLE_HPP

#include <cstdlib>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "redblack_impl.hpp"

template <typename T, typename Hash = std::hash<T>>
class HashTable {
 public:
  HashTable();
  HashTable(const HashTable<T, Hash>& other);
  HashTable(HashTable&& other) noexcept;
  ~HashTable();

  HashTable<T, Hash>& operator=(const HashTable<T, Hash>& other);
  HashTable<T, Hash>& operator=(HashTable<T, Hash>&& other) noexcept;
  bool operator==(const HashTable& other) const;
  bool operator!=(const HashTable& other) const;

  void addelem(const T& str);
  bool iselem(const T& str) const;
  size_t getfreq(const T& str) const;

  size_t get_counter() const;
  size_t get_size() const;

  template <class U>
  friend void change_realloc_size(HashTable<U>& h, double rhs, double startval);

  template <class U>
  friend void change_realloc_perc(HashTable<U>& h, double rhs, double startval);

 private:
  void hash_realloc();
  void tree_rehash(RBTree<T>& tree, std::vector<size_t>& rem);
  size_t gethash(const T& str) const;

  size_t START_HASHTABLE_SIZE = 8192;
  double ratio_to_realloc_ = 0.3;
  double val_for_realloc_ = 3.4;
  size_t size_;
  size_t counter_;

 protected:
  std::set<size_t> indexes_;
  RBTree<T>* table_;
};

#endif  // HASHTABLE_INCLUDES_HASH_TABLE_HPP