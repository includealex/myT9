#include <gtest/gtest.h>

#include "dictionary.hpp"

static const size_t nTests = 1e5;

std::string randstring_gen(size_t wsize) {
  static const char alphanum[] = "0123456789QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm";
  std::string res;

  res.reserve(wsize);
  for (size_t i = 0; i < wsize; ++i) {
    res += alphanum[rand() % (sizeof(alphanum) - 1)];
  }

  return res;
}

TEST(DictionaryTest, Crushttest) {
  size_t wsize = 5;
  Dictionary first(wsize);

  for (size_t i = 0; i < nTests; ++i) {
    first.addelem(randstring_gen(wsize));
  }
}

TEST(DictionaryTest, MostFit) {
  size_t wsize = 5;
  Dictionary first(wsize);

  for (size_t i = 0; i < nTests; ++i) {
    first.addelem(randstring_gen(wsize));
  }

  auto val = "hello";
  for (size_t i = 0; i <= nTests; ++i) {
    first.addelem(val);
  }

  std::string str = "hellod";
  auto ans = first.find_best_word(str);

  ASSERT_TRUE(ans.ldist >= 0);
  ASSERT_TRUE(ans.freq >= nTests);
  ASSERT_EQ(ans.str, val);

  ans = first.find_best_word(val);
  ASSERT_EQ(ans.ldist, 0);
  ASSERT_TRUE(ans.freq >= nTests);
  ASSERT_EQ(ans.str, val);
}

TEST(DictionaryTest, GetWordSize) {
  size_t wsize = rand() % nTests;
  Dictionary first(wsize);

  ASSERT_EQ(first.get_wordsize(), wsize);
}

int main(int argc, char* argv[]) {
  srand(time(nullptr));
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}