#include "editor.hpp"
#include <algorithm>
#include <fstream>

Editor::Editor() {
  dicts_ = new std::vector<Dictionary*>;
}

Editor::Editor(const Editor& other) {
  dicts_ = new std::vector<Dictionary*>;

  for(auto ind : *other.dicts_) {
    auto dict_to_copy = new Dictionary;
    *dict_to_copy = *ind;
    dicts_->push_back(dict_to_copy);
  }
}

Editor::Editor(Editor&& other) noexcept {
  dicts_ = other.dicts_;
  
  other.dicts_ = nullptr;
}

Editor::~Editor() {
  if(dicts_ == nullptr)
    return;

  for(auto ind = dicts_->begin(); ind < dicts_->end(); ++ind)
    delete *ind;

  delete dicts_;
}

Editor& Editor::operator=(const Editor& other) {
  if(this == &other)
    return *this;

  for(auto ind : *other.dicts_) {
    auto wsize = ind->get_wordsize();
    auto arr = other.get_dict_words(wsize);

    for(auto val : arr) {
      add_word(val);
    }
  }

  return* this;
}

Editor& Editor::operator=(Editor&& other) noexcept {
  if(this == &other)
    return *this;

  dicts_ = other.dicts_;
  other.dicts_ = nullptr;

  return *this;
}

bool Editor::operator==(const Editor& other) const {
  if(dicts_->size() != other.dicts_->size())
    return false;
  
  std::vector<size_t> arr;

  for(auto ind : *dicts_) {
    auto tsize = ind->get_wordsize();
    arr.push_back(tsize);
  }

  for(auto sz : arr) {
    auto farr = get_dict_words(sz);
    auto sarr = other.get_dict_words(sz);
    std::sort(farr.begin(), farr.end());
    std::sort(sarr.begin(), sarr.end());

    for(size_t i = 0; i < farr.size(); ++i) {
      if(farr[i] != sarr[i]) {
        return false;
      }
    }
  }

  return true;
}

bool Editor::operator!=(const Editor& other) const {
  return !(*this == other);
}

void Editor::add_dict(const size_t& wsize) {
  auto el = new Dictionary;
  el->set_wordsize(wsize);

  dicts_->push_back(el);
}

bool Editor::acceptable(const char& letter) const {
  if ((letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z'))
    return true;

  return false;
}

size_t Editor::filter_word(std::string* str) const{
  std::string res = "";

  for(auto letter : *str) {
    if(acceptable(letter))
        res += letter;
  }

  *str = res;
  return res.size();
}

bool Editor::is_dict(const size_t& wsize) const {
  for(auto ind : *dicts_) {
    if(ind->get_wordsize() == wsize)
      return true;
  }

  return false;
}

void Editor::add_word(const std::string& str) {
  auto val = str;
  auto wordsize = filter_word(&val);

  if(!wordsize)
    return;

  if(!is_dict(wordsize))
    add_dict(wordsize);

  for(auto ind : *dicts_) {
    if(ind->get_wordsize() == wordsize) {
      ind->addelem(val);
      return;
    }
  }
}

size_t Editor::total_words() const {
  if(dicts_ == nullptr)
    return 0;

  size_t res = 0;

  for(auto ind : *dicts_) {
    res += ind->get_counter();
  }

  return res;
}

std::vector<std::string> Editor::get_dict_words(const size_t& wsize) const {
  Dictionary* val;

  for(auto ind : *dicts_) {
    if(ind->get_wordsize() == wsize) {
      val = ind;
      break;
    }
  }

  auto res = val->get_all_elements();

  return res;
}

void Editor::print_dict_words(const size_t& wsize) const {
  auto res = get_dict_words(wsize);

  for(auto n : res) {
    std::cout << n << " ";
  }

  std::cout << std::endl;
}

std::string Editor::find_fit_word(const std::string& str) const {
  auto nstr = str;
  auto wsize = filter_word(&nstr);
  std::string ans = "";

  if(!wsize || wsize == 1)
    return str;

  bool* ds = new bool[3];

  for(int i = -1; i <= 1; ++i) {
    ds[i + 1] = is_dict(wsize + i);
  }

  cmp_word* arr = new cmp_word[3];

  for(int i = -1; i <= 1; ++i) {
    if(ds[i + 1]) {
      Dictionary* val;

      for(auto ind : *dicts_) {
        if(ind->get_wordsize() == wsize + i) {
        val = ind;
        break;
        }
      }      

      arr[i + 1] = val->find_best_word(str);
    }
    
    else {
      arr[i + 1].freq = 0;
      arr[i + 1].ldist = (wsize + i) * 2 + 1;
      arr[i + 1].str = "";
    }
  }

  auto val = std::min({arr[0], arr[1], arr[2]}, [wsize](const cmp_word& val1, const cmp_word& val2) {
    if(val1.ldist < val2.ldist)
      return true;
    if(val1.ldist > val2.ldist)
      return false;
    
    if(val1.freq > val2.freq)
      return true;
    if(val1.freq < val2.freq)
      return false;
    
    if(val1.str.size() == wsize)
      return true;
    if(val2.str.size() == wsize)
      return false;

    return true;
  });

  ans = val.str;

  delete[] arr;
  delete[] ds;
  return ans;
}

void Editor::teach(const std::string& filepath) {
  std::string pth = "../../../Books/";
  std::ifstream ifstrm(pth + filepath);

  if(!ifstrm.is_open()) {
    std::cout << "No file given" << std::endl;
    return;
  }
  
  while (!ifstrm.eof()) {
    std::string rhs;
    ifstrm >> rhs;

    add_word(rhs);
  }

  ifstrm.close();
}

void Editor::edit(const std::string& filename) {
  auto path = "../../../" + filename;
  std::ifstream ifstrm(path);
  std::vector<std::string> buf;

  if(!ifstrm.is_open()) {
    std::cout << "No file given" << std::endl;
    return;
  }

  while(!ifstrm.eof()) {
    std::string rhs;
    ifstrm >> rhs;

    buf.push_back(find_fit_word(rhs));    
  }
  ifstrm.close();

  std::ofstream file(path);
  for(auto str : buf) {
    file << (str + " ");
  }

  file.close();
}

void Editor::thredit(const std::string& filename) {
  size_t num = 10;
  auto path = "../../../" + filename;
  std::vector<std::vector<std::string>> buf(num);

  std::ifstream ifstrm(path);
  size_t counter = 0;

  if(!ifstrm.is_open()) {
    std::cout << "No file given" << std::endl;
    return;
  }

  while(!ifstrm.eof()) {
    std::string val;
    ifstrm >> val;
    ++counter;
  }

  ifstrm.close();

  ifstrm.open(path);

  while(!ifstrm.eof()) {
    std::string rhs;
    ifstrm >> rhs;
    auto n = counter % num;
    ++counter;

    buf[n].push_back(rhs);
  }

  if(counter < num)
    num = counter;

  ifstrm.close();

  auto arr = new std::thread[num];

  auto foo = [&](size_t num, std::vector<std::string>& vec) { 
    std::vector<std::string> resarr;

    for(auto el : vec) {
      auto res = find_fit_word(el);
      resarr.push_back(res);        
    }
    
    vec.erase(vec.begin());

    for(auto el : resarr)
      vec.push_back(el);
  };

  for(size_t i = 0; i < num; ++i) {
    arr[i] = std::thread(foo, std::ref(i), std::ref(buf[i]));
  }

  for (size_t i = 0; i < num; ++i) {
    arr[i].join();
  }
  
  delete[] arr;  

  std::vector<std::string> result;
  for(auto j : buf) {
    for(auto i : j)
      result.push_back(i);
  }

  std::ofstream file(path);
  for(auto str : result) {
    file << (str + " ");
  }

  file.close();
}