#include <gtest/gtest.h>

#include "editor.hpp"

static const size_t nTests = 100;

std::string randstring_gen(size_t wsize) {
  static const char alphanum[] = "QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm";
  std::string res;

  res.reserve(wsize);
  for (size_t i = 0; i < wsize; ++i) {
    res += alphanum[rand() % (sizeof(alphanum) - 1)];
  }

  return res;
}

TEST(EditorTest, CrushTest) {
  Editor first;

  for(size_t i = 1; i <= nTests; ++i) {
    first.add_word(randstring_gen(i));
  }
}

TEST(EditorTest, CopyConstructor) {
  Editor first;

  for(size_t i = 2; i <= nTests; ++i) {
    first.add_word(randstring_gen(i));
  }

  Editor second(first);

  ASSERT_EQ(first.total_words(), second.total_words());
  
  std::string a = "a";
  first.add_word(a);
  ASSERT_NE(first.total_words(), second.total_words());
}

TEST(EditorTest, MoveConstructor) {
  Editor first;

  for(size_t i = 1; i <= nTests; ++i) {
    first.add_word(randstring_gen(i));
  }
  auto val = first.total_words();

  Editor second(std::move(first));

  ASSERT_EQ(first.total_words(), 0);
  ASSERT_EQ(second.total_words(), val);
}

TEST(EditorTest, CopyOperator) {
  Editor first;

  for(size_t i = 1; i <= nTests; ++i) {
    first.add_word(randstring_gen(i));
  }

  Editor second = first;

  ASSERT_EQ(first.total_words(), second.total_words());
}

TEST(EditorTest, MoveOperator) {
  Editor first;

  for(size_t i = 1; i <= nTests; ++i) {
    first.add_word(randstring_gen(i));
  }
  auto val = first.total_words();

  Editor second = std::move(first);

  ASSERT_EQ(first.total_words(), 0);
  ASSERT_EQ(second.total_words(), val);
}

TEST(EditorTest, EqOperator) {
  Editor first;
  Editor second;

  for(size_t i = 2; i <= nTests; ++i) {
    auto val = randstring_gen(i);

    first.add_word(val);
    second.add_word(val);
  }

  ASSERT_TRUE(first == second);

  first.add_word(randstring_gen(1));
  ASSERT_FALSE(first == second);
}

TEST(EditorTest, IsDict) {
  Editor first;

  for(size_t i = 1; i < nTests; ++i)
    first.add_word(randstring_gen(i));

  for(size_t i = 1; i < nTests; ++i) {
    ASSERT_TRUE(first.is_dict(i));
  }

  ASSERT_FALSE(first.is_dict(nTests));
}

TEST(EditorTest, NumberOfWords) {
  Editor first;

  for(size_t i = 1; i <= nTests; ++i) {
    first.add_word(randstring_gen(i));
  }

  ASSERT_TRUE(first.total_words() <= nTests);
}

TEST(EditorTest, GetDictWords) {
  Editor first;
  std::vector<std::string> arr;
  size_t randsize = rand() % nTests + 2;

  for(size_t i = 0; i < nTests; ++i) {
    auto val = randstring_gen(randsize);
    first.add_word(val);
    bool isinarr = false;
    
    for(auto iter : arr) {
      if(iter == val)
        isinarr = true;
    }

    if(!isinarr)
      arr.push_back(val);
  }

  auto res = first.get_dict_words(randsize);
  std::sort(res.begin(), res.end());
  std::sort(arr.begin(), arr.end());

  ASSERT_EQ(arr.size(), res.size());

  for(size_t i = 0; i < arr.size(); ++i) {
    ASSERT_EQ(arr[i], res[i]);
  }
}

TEST(EditorTest, GetFitWord) {
  Editor first;
  size_t sz = rand() % nTests + 2;
  std::string diff = randstring_gen(sz - 1);
  std::string val = diff + "o";
  std::string start = diff + "p";

  for(size_t i = 0; i <= nTests + 1; ++i) {
    first.add_word(val);
  }

  for(size_t i = 0; i <= nTests; ++i) {
    std::string randstr = "";
    randstr = randstring_gen(sz);
    if(randstr != start)
      first.add_word(randstr);
  }

  ASSERT_EQ(first.find_fit_word(start), val);

  first.add_word(start);
  ASSERT_EQ(first.find_fit_word(start), start);
}

TEST(EditorTest, TeachingCrush) {
  Editor first;

  first.teach("LOR.txt");
  first.teach("Onegin.txt");
}

int main(int argc, char* argv[]) {
  srand(time(nullptr));
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}