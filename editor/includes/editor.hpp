#ifndef EDITOR_INCLUDES_EDITOR_HPP
#define EDITOR_INCLUDES_EDITOR_HPP

#include "dictionary.hpp"

#include <vector>
#include <set>
#include <thread>

class Editor final{
 public:
  Editor();
  Editor(const Editor& other);
  Editor(Editor&& other) noexcept;
  ~Editor();
  
  Editor& operator=(const Editor& other);
  Editor& operator=(Editor&& other) noexcept;
  bool operator==(const Editor& other) const;
  bool operator!=(const Editor& other) const;

  void add_word(const std::string& str);
  bool is_dict(const size_t& wsize) const;
  size_t total_words() const;

  void teach(const std::string& filepath);
  void edit(const std::string& filepath);
  void thredit(const std::string& filepath);

  std::string find_fit_word(const std::string& str) const;

  std::vector<std::string> get_dict_words(const size_t& wsize) const;
  void print_dict_words(const size_t& wsize) const;

 private:
  std::vector<Dictionary*>* dicts_;

  void add_dict(const size_t& wsize);
  bool acceptable(const char& letter) const;
  size_t filter_word(std::string* str) const;
};

#endif //EDITOR_INCLUDES_EDITOR_HPP