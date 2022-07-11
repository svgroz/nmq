#include <gtest/gtest.h>

#include <cstring>

#include <nmq/core/key_value.h>

TEST(KeyValueTest, HappyPathEmptyKV) {
  nmq::KeyValueHeader x[] = {0, 0};

  auto kv = nmq::KeyValue::read((char *)x, sizeof(x));

  EXPECT_EQ(kv->has_key(), false);
  EXPECT_EQ(kv->key().size(), 0);
  EXPECT_EQ(kv->has_value(), false);
  EXPECT_EQ(kv->value().size(), 0);
}

TEST(KeyValueTest, HappyPathKNullVNull) {

  nmq::KeyValueHeader x[] = {-1, -1};

  auto kv = nmq::KeyValue::read((char *)x, sizeof(x));

  EXPECT_EQ(kv->has_key(), false);
  EXPECT_EQ(kv->key().size(), 0);
  EXPECT_EQ(kv->has_value(), false);
  EXPECT_EQ(kv->value().size(), 0);
}

TEST(KeyValueTest, HappyPathKVNull) {

  nmq::KeyValueHeader x[] = {4, 0, -1};

  auto kv = nmq::KeyValue::read((char *)x, sizeof(x));

  EXPECT_EQ(kv->has_key(), true);
  EXPECT_EQ(kv->key().size(), 4);
  EXPECT_EQ(kv->has_value(), false);
  EXPECT_EQ(kv->value().size(), 0);
}

TEST(KeyValueTest, HappyPathKNullV) {

  nmq::KeyValueHeader x[] = {-1, 4, 0};

  auto kv = nmq::KeyValue::read((char *)x, sizeof(x));

  EXPECT_EQ(kv->has_key(), false);
  EXPECT_EQ(kv->key().size(), 0);
  EXPECT_EQ(kv->has_value(), true);
  EXPECT_EQ(kv->value().size(), 4);
}

TEST(KeyValueTest, HappyPathKV) {

  nmq::KeyValueHeader x[] = {4, 4, 1, 1};

  auto kv = nmq::KeyValue::read((char *)x, sizeof(x));

  EXPECT_EQ(kv->has_key(), true);
  EXPECT_EQ(kv->key().size(), 4);
  EXPECT_EQ(kv->has_value(), true);
  EXPECT_EQ(kv->value().size(), 4);
}
