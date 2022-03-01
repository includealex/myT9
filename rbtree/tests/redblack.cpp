#include "../includes/redblack.hpp"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <gtest/gtest.h>

#include "../includes/redblack_impl.hpp"

static const size_t nTests = 1e6;

TEST(RBTreeTest, Crushtest) {
  RBTree<int> first;
  for(size_t i = 0; i < nTests; ++i) {
    int x = rand() % nTests;
    first.add(x);
  }

  for(size_t i = 0; i < nTests; ++i) {
    first.add(i);
  }
}

TEST(RBTreeTest, Size) {
  RBTree<int> first;

  for(size_t i = 0; i < nTests; ++i) {
    first.add(i);
  }

  ASSERT_EQ(first.getsize(), nTests);
}

TEST(RBTreeTest, Search) {
  RBTree<int> first;
  int val;
  size_t num = rand() % nTests;

  for(size_t i = 0; i < nTests; ++i) {
    first.add(i);
    if(i == num) {
      val = nTests + rand() % nTests;
      first.add(val);
    }
  }

  ASSERT_TRUE(first.search(val));
}

TEST(RBTreeTest, Equality) {
  RBTree<int> first;
  RBTree<int> second;

  for(size_t i = 0; i < nTests; ++i) {
    first.add(i);
    second.add(i);
  }

  ASSERT_TRUE(first == second);
}

TEST(RBTreeTest, IsEmpty) {
  RBTree<int> first;
  ASSERT_TRUE(first.is_empty());
  int x = rand() % nTests;
  first.add(x);
  ASSERT_FALSE(first.is_empty());
}

int main(int argc, char* argv[]) {
  srand(time(nullptr));
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}