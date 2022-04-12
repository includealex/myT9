#include "dictionary.hpp"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <new>

Dictionary::Dictionary(size_t wsize) : wordsize_(wsize) {}

size_t Dictionary::lev_dist(const std::string& first, const std::string& second) const {
  size_t n = first.size() + 1;
  size_t m = second.size() + 1;
  size_t resstr;

  auto distance = new size_t[n * m];

  distance[0] = 0;
  for (size_t i = 1; i < n; ++i) {
    distance[i] = i;
  }

  for (size_t j = 1; j < m; ++j) {
    distance[j * n] = j;
  }

  for (size_t j = 1; j < m; ++j) {
    for (size_t i = 1; i < n; ++i) {
      size_t indicator = first[i - 1] != second[j - 1] ? 1 : 0;

      distance[i + j * n] =
          std::min({distance[i - 1 + j * n] + 1,
                    distance[i + (j - 1) * n] + 1,
                    distance[i - 1 + (j - 1) * n] + indicator},
                   [](const size_t& val1, const size_t& val2) { return val1 < val2; });
    }
  }

  resstr = distance[m * n - 1];

  delete[] distance;
  return resstr;
}

cmp_word Dictionary::find_best_word(const std::string& str) const {
  cmp_word ans;
  if (iselem(str)) {
    ans.ldist = 0;
    ans.str = str;
    ans.freq = getfreq(str);
    return ans;
  }

  size_t ldist = 2 * wordsize_ + 1;
  size_t freq = 0;
  std::string res = "";

  for (auto n : indexes_) {
    std::vector<std::string> arr = table_[n].get_inorder();
    for (auto el : arr) {
      size_t dist = 0;
      dist = lev_dist(str, el);
      if (dist < ldist) {
        ldist = dist;
        res = el;
        freq = table_[n].get_frequency(el);
      }
      if (dist == ldist) {
        auto nfreq = table_[n].get_frequency(el);
        if (freq < nfreq) {
          res = el;
          freq = nfreq;
        }
      }
    }
  }

  ans.ldist = ldist;
  ans.freq = freq;
  ans.str = res;

  return ans;
}

size_t Dictionary::get_wordsize() const {
  return wordsize_;
}

void Dictionary::set_wordsize(const size_t& wsize) {
  wordsize_ = wsize;
}