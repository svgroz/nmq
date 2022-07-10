#include <gtest/gtest.h>

#include <cstring>

#include <nmq/core/key_value.h>

TEST(KeyValueTest, HappyPathEmptyKV) {
  char x[sizeof(std::int32_t) * 2] = {0};
  auto kv = nmq::KeyValue::read(x, sizeof(x));

  EXPECT_EQ(kv->key_size(), 0);
  EXPECT_EQ(kv->key().size(), 0);
  EXPECT_EQ(kv->value_size(), 0);
  EXPECT_EQ(kv->value().size(), 0);
}

TEST(KeyValueTest, HappyPathKNullVNull) {

  std::int32_t x[] = {-1, -1};

  auto kv = nmq::KeyValue::read((char *)x, sizeof(x));

  EXPECT_LE(kv->key_size(), 0);
  EXPECT_LE(kv->value_size(), 0);
}

TEST(KeyValueTest, HappyPathKVNull) {

  std::int32_t x[] = {4, 0, -1};

  auto kv = nmq::KeyValue::read((char *)x, sizeof(x));

  EXPECT_EQ(kv->key_size(), 4);
  EXPECT_LE(kv->value_size(), 0);
}

TEST(KeyValueTest, HappyPathKNullV) {

  std::int32_t x[] = {0, 4, 0};

  auto kv = nmq::KeyValue::read((char *)x, sizeof(x));

  EXPECT_LE(kv->key_size(), 0);
  EXPECT_EQ(kv->value_size(), 4);
}

TEST(KeyValueTest, HappyPathKV) {

  std::int32_t x[] = {4, 4, 1, 1};

  auto kv = nmq::KeyValue::read((char *)x, sizeof(x));

  EXPECT_EQ(kv->key_size(), 4);
  EXPECT_EQ(kv->value_size(), 4);
}