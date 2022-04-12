#include <gtest/gtest.h>

#include <iostream>

#include "hash_table.hpp"
#include "hash_table_impl.hpp"

static const size_t nTests = 1e4;

TEST(HashTableTest, CrushTest) {
  HashTable<int> first;

  for (size_t i = 0; i < nTests; ++i) {
    first.addelem(i);
  }
}

TEST(HashTableTest, CopyConstructor) {
  HashTable<int> first;

  for (size_t i = 0; i < nTests; ++i) {
    first.addelem(i);
  }

  HashTable<int> second(first);

  ASSERT_EQ(first.get_counter(), second.get_counter());
  ASSERT_EQ(first.get_size(), second.get_size());
}

TEST(HashTableTest, MoveConstructor) {
  HashTable<int> first;

  for (size_t i = 0; i < nTests; ++i) {
    first.addelem(i);
  }

  HashTable<int> second(std::move(first));
  ASSERT_EQ(first.get_counter(), 0);
  ASSERT_NE(second.get_counter(), 0);
  ASSERT_EQ(first.get_size(), 0);
  ASSERT_NE(second.get_size(), 0);
}

TEST(HashTableTest, CopyOperator) {
  HashTable<int> first;
  HashTable<int> second;

  for (size_t i = 0; i < nTests; ++i) {
    first.addelem(i);
  }

  second = first;
  ASSERT_EQ(first.get_counter(), second.get_counter());
  ASSERT_EQ(first.get_size(), second.get_size());
}

TEST(HashTableTest, MoveOperator) {
  HashTable<int> first;
  HashTable<int> second;

  for (size_t i = 0; i < nTests; ++i) {
    first.addelem(i);
  }
  ASSERT_EQ(second.get_counter(), 0);

  second = std::move(first);
  ASSERT_NE(second.get_counter(), 0);
}

TEST(HashTableTest, EqOperator) {
  HashTable<int> first;
  HashTable<int> second;
  int val = rand() % nTests + nTests;

  for (size_t i = 0; i < nTests; ++i) {
    first.addelem(i);
    second.addelem(i);
    ASSERT_TRUE(first == second);
  }

  first.addelem(val);
  ASSERT_FALSE(first == second);
}

TEST(HashTableTest, GetHash) {
  HashTable<std::string> first;

  ASSERT_FALSE(first.iselem("val"));

  for (size_t i = 0; i < nTests; ++i) {
    first.addelem("val");

    ASSERT_TRUE(first.iselem("val"));
  }
}

TEST(HashTableTest, Counter) {
  HashTable<int> first;
  ASSERT_EQ(first.get_counter(), 0);

  int val = rand() % nTests;
  first.addelem(val);

  ASSERT_EQ(first.get_counter(), 1);

  first.addelem(val);
  ASSERT_EQ(first.get_counter(), 1);
}

TEST(HashTableTest, Iselem) {
  HashTable<int> first;

  for (size_t i = 0; i < nTests; ++i) {
    auto val = rand() % nTests;

    first.addelem(val);

    ASSERT_TRUE(first.iselem(val));
  }
}

TEST(HashTableTest, GetFrequency) {
  HashTable<int> first;

  for (size_t i = 0; i < nTests; ++i) {
    first.addelem(i);
  }
  for (size_t i = 0; i < nTests; ++i) {
    ASSERT_EQ(first.getfreq(i), 1);
  }

  ASSERT_EQ(first.getfreq(nTests), 0);
  auto val = rand() % nTests;
  for (size_t i = 0; i < nTests; ++i) {
    first.addelem(val);
  }

  ASSERT_EQ(first.getfreq(val), nTests + 1);
}

TEST(HashTableTest, GetAllElements) {
  HashTable<int> first;

  for (size_t i = 0; i < nTests; ++i) {
    first.addelem(i);
  }

  auto res = first.get_all_elements();
  ASSERT_EQ(res.size(), nTests);

  for(size_t i = 0; i < nTests; ++i) {
    auto val = std::find(res.begin(), res.end(), i);
    ASSERT_EQ(*val, i);
  }
}

int main(int argc, char* argv[]) {
  srand(time(nullptr));
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}