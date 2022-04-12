#ifndef DICTIONARY_INCLUDES_DICTIONARY_HPP
#define DICTIONARY_INCLUDES_DICTIONARY_HPP

#include <cstring>

#include "hash_table_impl.hpp"

struct cmp_word {
  size_t ldist;
  size_t freq;
  std::string str;
};

class Dictionary : public HashTable<std::string> {
 public:
  explicit Dictionary(size_t wsize = 0);
  cmp_word find_best_word(const std::string& str) const;
  size_t get_wordsize() const;
  void set_wordsize(const size_t& wsize);

 private:
  size_t lev_dist(const std::string& first, const std::string& second) const;

  size_t wordsize_;
};

#endif  // DICTIONARY_INCLUDES_DICTIONARY_HPP