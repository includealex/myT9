#include "../includes/redblack.hpp"

#include <gtest/gtest.h>

#include <cstdlib>
#include <ctime>
#include <iostream>

#include "../includes/redblack_impl.hpp"

static const size_t nTests = 1e5;

TEST(RBTreeTest, Crushtest) {
  RBTree<int> first;
  for (size_t i = 0; i < nTests; ++i) {
    int x = rand() % nTests;
    first.add(x);
  }

  for (size_t i = 0; i < nTests; ++i) {
    first.add(i);
  }
}

TEST(RBTreeTest, Size) {
  RBTree<int> first;

  for (size_t i = 0; i < nTests; ++i) {
    first.add(i);
  }

  ASSERT_EQ(first.getsize(), nTests);
}

TEST(RBTreeTest, Search) {
  RBTree<int> first;
  int val;
  size_t num = rand() % nTests;

  for (size_t i = 0; i < nTests; ++i) {
    first.add(i);
    if (i == num) {
      val = nTests + rand() % nTests;
      first.add(val);
    }
  }

  ASSERT_TRUE(first.search(val));
}

TEST(RBTreeTest, Equality) {
  RBTree<int> first;
  RBTree<int> second;

  int x = rand() % nTests;
  int y = x + 1 + (rand() % nTests);

  for (size_t i = 0; i < nTests; ++i) {
    first.add(i);
    second.add(i);
  }

  ASSERT_TRUE(first == second);

  first.add(1);
  ASSERT_FALSE(first == second);

  second.add(1);
  first.add(x);

  ASSERT_FALSE(first == second);

  second.add(y);

  ASSERT_FALSE(first == second);
}

TEST(RBTreeTest, IsEmpty) {
  RBTree<int> first;
  ASSERT_TRUE(first.is_empty());
  int x = rand() % nTests;
  first.add(x);
  ASSERT_FALSE(first.is_empty());
}

TEST(RBTreeTest, FrequencyCrushTest) {
  RBTree<std::string> first;
  std::string str = "hello, people";

  for (size_t i = 0; i < nTests; ++i) {
    first.add(str);
  }
}

TEST(RBTreeTest, MostFrequent) {
  RBTree<int> first;
  int val = rand() % nTests;

  for(size_t i = 0; i < nTests; ++i) {
    first.add(i);
  }

  first.add(val);

  ASSERT_EQ(first.find_most_frequent(), val);

  int qval = rand() % nTests;

  for(size_t i = 0; i < nTests; ++i) {
	  first.add(val);
	  first.add(qval);
  }

  ASSERT_EQ(first.find_most_frequent(), val);
}

TEST(RBTreeTest, MoveConstructor) {
  RBTree<int> first;
  RBTree<int> second;

  for(size_t i = 0; i < nTests; ++i) {
    first.add(i);
  }

  second = std::move(first);

  ASSERT_EQ(nTests, second.getsize());
}

int main(int argc, char* argv[]) {
  srand(time(nullptr));
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}