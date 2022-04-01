#include "redblack.hpp"

#include <gtest/gtest.h>

#include <cstdlib>
#include <ctime>
#include <iostream>

#include "redblack_impl.hpp"

static const size_t nTests = 1e4;

TEST(NodeT, CopyConstructor) {
  int val = rand() % nTests;

  NodeT<int> first;
  first.data_ = val;
  NodeT<int> second;

  int secval = rand() % nTests + nTests;
  second.data_ = secval;
  first.left_ = &second;

  NodeT<int> third(first);

  ASSERT_EQ(third.data_, val);
  ASSERT_EQ(third.left_->data_, secval);
}

TEST(NodeT, MoveConstructor) {
  int val = rand() % nTests;

  NodeT<int> first;
  first.data_ = val;

  NodeT<int> second(std::move(first));

  ASSERT_EQ(second.data_, val);
}

TEST(RBTreeTest, Crushtest) {
  RBTree<int> first;

  for (size_t i = 0; i < nTests; ++i) {
    first.add(i);
  }

  for (size_t i = 0; i < nTests; ++i) {
    first.delete_elem(i);
  }
}

TEST(RBTreeTest, Size) {
  RBTree<int> first;

  for (size_t i = 0; i < nTests; ++i) {
    first.add(i);
  }

  ASSERT_EQ(first.getsize(), nTests);
}

TEST(RBTreeTest, IsElem) {
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

  ASSERT_TRUE(first.iselem(val));
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

  for (size_t i = 0; i < nTests; ++i) {
    first.add(i);
  }

  first.add(val);

  ASSERT_EQ(first.find_most_frequent(), val);

  int qval = rand() % nTests;

  for (size_t i = 0; i < nTests; ++i) {
    first.add(val);
    first.add(qval);
  }

  ASSERT_EQ(first.find_most_frequent(), val);
}

TEST(RBTreeTest, CopyConstructor) {
  RBTree<int> first;

  for (size_t i = 0; i < nTests; ++i) {
    first.add(i);
  }

  RBTree<int> second(first);
  ASSERT_EQ(second.getsize(), nTests);
  ASSERT_EQ(first.getsize(), nTests);
}

TEST(RBTreeTest, MoveAndEqConstructor) {
  RBTree<int> first;
  RBTree<int> second;

  for (size_t i = 0; i < nTests; ++i) {
    first.add(i);
  }

  second = std::move(first);

  ASSERT_EQ(nTests, second.getsize());
}

TEST(RBTreeTest, MoveConstructor) {
  RBTree<int> first;

  for (size_t i = 0; i < nTests; ++i) {
    first.add(i);
  }

  RBTree<int> second(std::move(first));
  ASSERT_EQ(second.getsize(), nTests);
}

TEST(RBTreeTest, Inorder) {
  RBTree<int> first;

  for (size_t i = 0; i < nTests; ++i) {
    first.add(i);
  }

  auto res = first.get_inorder();
  for (size_t i = 1; i < first.getsize(); ++i) {
    ASSERT_TRUE(res[i] > res[i - 1]);
  }
}

TEST(RBTreeTest, GetFrequency) {
  RBTree<int> first;

  for (size_t i = 0; i < nTests; ++i) {
    first.add(i);
  }

  int elem = rand() % nTests;
  ASSERT_EQ(first.get_frequency(elem), 1);

  for (size_t i = 1; i < nTests; ++i) {
    first.add(elem);
  }

  ASSERT_EQ(first.get_frequency(elem), nTests);
}

TEST(RBTreeTest, SetFrequency) {
  RBTree<int> first;

  int num = rand() % nTests;
  int newfreq = rand() % nTests + nTests;

  for (size_t i = 0; i < nTests; ++i) {
    first.add(i);
  }

  first.set_frequency(num, newfreq);

  ASSERT_EQ(first.get_frequency(num), newfreq);
}

int main(int argc, char* argv[]) {
  srand(time(nullptr));
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}