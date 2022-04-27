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
  auto path = "../../../" + filename;
  size_t nthreads = 5;

  std::vector<std::string> readen;
  std::vector<std::string> written;
  std::ifstream ifstrm(path);

  if(!ifstrm.is_open()) {
    std::cout << "No file given" << std::endl;
    return;
  }

  while(!ifstrm.eof()) {
    std::string val;
    ifstrm >> val;

    readen.push_back(val);
    written.push_back("");
  }
  ifstrm.close();

  size_t nwords = readen.size();

  auto threadfoo = [&](size_t i) {
    while(i < nwords) {
      written[i] = find_fit_word(readen[i]);
      i += nthreads; 
    }
  };

  auto arr = new std::thread[nthreads];
  for(size_t i = 0; i < nthreads; ++i) {
    arr[i] = std::thread(threadfoo, i);
  }

  for (size_t i = 0; i < nthreads; ++i) {
    arr[i].join();
  }  
  delete[] arr;  

  std::ofstream file(path);
  for(auto str : written) {
    file << (str + " ");
  }

  file.close();
}