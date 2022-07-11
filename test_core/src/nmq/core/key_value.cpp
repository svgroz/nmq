#include <gtest/gtest.h>

#include <cstring>

#include <nmq/core/key_value.h>

TEST(KeyValueTest, HappyPathEmptyKV) {
  std::int32_t x[] = {0, 0};

  auto kv = nmq::KeyValue::read((char *)x, sizeof(x));

  EXPECT_EQ(kv->has_key(), false);
  EXPECT_EQ(kv->key().size(), 0);
  EXPECT_EQ(kv->has_value(), false);
  EXPECT_EQ(kv->value().size(), 0);
}

TEST(KeyValueTest, HappyPathKNullVNull) {

  std::int32_t x[] = {-1, -1};

  auto kv = nmq::KeyValue::read((char *)x, sizeof(x));

  EXPECT_EQ(kv->has_key(), false);
  EXPECT_EQ(kv->key().size(), 0);
  EXPECT_EQ(kv->has_value(), false);
  EXPECT_EQ(kv->value().size(), 0);
}

TEST(KeyValueTest, HappyPathKVNull) {

  std::int32_t x[] = {4, 0, -1};

  auto kv = nmq::KeyValue::read((char *)x, sizeof(x));

  EXPECT_EQ(kv->has_key(), true);
  EXPECT_EQ(kv->key().size(), 4);
  EXPECT_EQ(kv->has_value(), false);
  EXPECT_EQ(kv->value().size(), 0);
}

TEST(KeyValueTest, HappyPathKNullV) {

  std::int32_t x[] = {-1, 4, 0};

  auto kv = nmq::KeyValue::read((char *)x, sizeof(x));

  EXPECT_EQ(kv->has_key(), false);
  EXPECT_EQ(kv->key().size(), 0);
  EXPECT_EQ(kv->has_value(), true);
  EXPECT_EQ(kv->value().size(), 4);
}

TEST(KeyValueTest, HappyPathKV) {

  std::int32_t x[] = {4, 4, 1, 1};

  auto kv = nmq::KeyValue::read((char *)x, sizeof(x));

  EXPECT_EQ(kv->has_key(), true);
  EXPECT_EQ(kv->key().size(), 4);
  EXPECT_EQ(kv->has_value(), true);
  EXPECT_EQ(kv->value().size(), 4);
}
