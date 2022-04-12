#ifndef HASHTABLE_INCLUDES_HASH_TABLE_IMPL_HPP
#define HASHTABLE_INCLUDES_HASH_TABLE_IMPL_HPP

#include <algorithm>

#include "hash_table.hpp"

template <class U>
void change_realloc_size(HashTable<U>& h, double rhs, double startval) {
  h.val_for_realloc_ = startval;
  h.val_for_realloc_ += rhs;
}

template <class U>
void change_realloc_perc(HashTable<U>& h, double rhs, double startval) {
  h.ratio_to_realloc_ = startval;
  h.ratio_to_realloc_ += rhs;
}

template <typename T, typename Hash>
HashTable<T, Hash>::HashTable() : size_(START_HASHTABLE_SIZE), counter_(0) {
  table_ = new RBTree<T>[size_];
}

template <typename T, typename Hash>
HashTable<T, Hash>::HashTable(const HashTable<T, Hash>& other) {
  size_ = other.size_;
  counter_ = other.counter_;
  indexes_ = other.indexes_;

  table_ = new RBTree<T>[size_];

  for (size_t i = 0; i < other.size_; ++i) {
    if (other.table_[i].getsize())
      table_[i] = other.table_[i];
  }
}

template <typename T, typename Hash>
HashTable<T, Hash>::HashTable(HashTable&& other) noexcept
    : size_(other.size_), counter_(other.counter_), indexes_(other.indexes_), table_(other.table_) {
  other.counter_ = 0;
  other.size_ = 0;
  other.indexes_.clear();
  other.table_ = nullptr;
}

template <typename T, typename Hash>
HashTable<T, Hash>::~HashTable() {
  indexes_.clear();
  delete[] table_;
}

template <typename T, typename Hash>
HashTable<T, Hash>& HashTable<T, Hash>::operator=(const HashTable<T, Hash>& other) {
  if (this == &other)
    return *this;

  delete[] table_;
  indexes_.clear();
  size_ = other.size_;
  counter_ = other.counter_;

  table_ = new RBTree<T>[size_];
  for (size_t i = 0; i < size_; ++i) {
    if (other.table_[i].getsize())
      table_[i] = other.table_[i];
  }

  return *this;
}

template <typename T, typename Hash>
HashTable<T, Hash>& HashTable<T, Hash>::operator=(HashTable<T, Hash>&& other) noexcept {
  if (this == &other)
    return *this;

  delete[] table_;
  indexes_.clear();
  size_ = other.size_;
  counter_ = other.counter_;

  table_ = other.table_;
  other.table_ = nullptr;
  indexes_ = std::move(other.indexes_);

  return *this;
}

template <typename T, typename Hash>
bool HashTable<T, Hash>::operator==(const HashTable<T, Hash>& other) const {
  if (size_ != other.size_)
    return false;
  if (counter_ != other.counter_)
    return false;
  if (indexes_ != other.indexes_)
    return false;

  for (size_t i = 0; i < size_; ++i) {
    if ((other.table_[i].getsize() && table_[i].getsize())) {
      if (other.table_[i] != table_[i])
        return false;
    }
  }

  return true;
}

template <typename T, typename Hash>
bool HashTable<T, Hash>::operator!=(const HashTable<T, Hash>& other) const {
  return !(*this == other);
}

template <typename T, typename Hash>
size_t HashTable<T, Hash>::gethash(const T& str) const {
  auto foo = Hash();

  auto res = foo(str) % size_;
  return res;
}

template <typename T, typename Hash>
void HashTable<T, Hash>::addelem(const T& str) {
  auto check_realloc = static_cast<size_t>(size_ * ratio_to_realloc_);
  if (counter_ > check_realloc) {
    hash_realloc();
  }

  size_t num = gethash(str);
  size_t sizeoftree = table_[num].getsize();

  table_[num].add(str);
  indexes_.insert(num);

  if (sizeoftree != table_[num].getsize())
    ++counter_;
}

template <typename T, typename Hash>
bool HashTable<T, Hash>::iselem(const T& str) const {
  size_t num = gethash(str);

  if (!table_[num].getsize()) {
    return false;
  }

  return table_[num].iselem(str);
}

template <typename T, typename Hash>
size_t HashTable<T, Hash>::getfreq(const T& str) const {
  if (!iselem(str))
    return 0;

  size_t num = gethash(str);

  return table_[num].get_frequency(str);
}

template <typename T, typename Hash>
size_t HashTable<T, Hash>::get_counter() const {
  return counter_;
}

template <typename T, typename Hash>
size_t HashTable<T, Hash>::get_size() const {
  return size_;
}

template <typename T, typename Hash>
void HashTable<T, Hash>::tree_rehash(RBTree<T>& tree, std::vector<size_t>& rem) {
  std::vector<T> elems = tree.get_inorder();

  for (auto n : elems) {
    auto val = gethash(n);
    table_[val].add(n);
    rem.push_back(val);
  }
}

template <typename T, typename Hash>
void HashTable<T, Hash>::hash_realloc() {
  auto mind_ = table_;
  auto newsize_ = static_cast<size_t>(size_ * val_for_realloc_);
  std::vector<size_t> rem;

  table_ = new RBTree<T>[newsize_];
  for (auto el : indexes_) {
    auto tree = mind_[el];
    tree_rehash(tree, rem);
  }

  indexes_.clear();
  for (auto el : rem) {
    indexes_.insert(el);
  }

  delete[] mind_;
  size_ = newsize_;
}

template <typename T, typename Hash>
std::vector<T> HashTable<T, Hash>::get_all_elements() const {
  std::vector<T> res;

  if(indexes_.empty())
    return res;

  for (auto i : indexes_) {
    std::vector<T> val = table_[i].get_inorder();

    for(auto n : val)
      res.push_back(n);
  }

  return res;
}

#endif  // HASHTABLE_INCLUDES_HASH_TABLE_IMPL_HPP