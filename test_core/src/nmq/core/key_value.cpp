#include <gtest/gtest.h>

#include <cstring>

#include <nmq/core/key_value.h>

TEST(KeyValueTest, HappyPathReadEmptyKV) {
  nmq::KeyValueHeader sourse_buffer[] = {0, 0};

  auto kv = nmq::KeyValue::read((char *)sourse_buffer, sizeof(sourse_buffer));

  EXPECT_EQ(kv.has_key(), false);
  EXPECT_EQ(kv.key_size(), 0);
  EXPECT_EQ(kv.has_value(), false);
  EXPECT_EQ(kv.value_size(), 0);
}

TEST(KeyValueTest, HappyPathReadKNullVNull) {

  nmq::KeyValueHeader sourse_buffer[] = {-1, -1};

  auto kv = nmq::KeyValue::read((char *)sourse_buffer, sizeof(sourse_buffer));

  EXPECT_EQ(kv.has_key(), false);
  EXPECT_EQ(kv.key_size(), 0);
  EXPECT_EQ(kv.has_value(), false);
  EXPECT_EQ(kv.value_size(), 0);
}

TEST(KeyValueTest, HappyPathReadKVNull) {

  nmq::KeyValueHeader sourse_buffer[] = {sizeof(nmq::KeyValueHeader), 0, -1};

  auto kv = nmq::KeyValue::read((char *)sourse_buffer, sizeof(sourse_buffer));

  EXPECT_EQ(kv.has_key(), true);
  EXPECT_EQ(kv.key_size(), 4);
  EXPECT_EQ(kv.has_value(), false);
  EXPECT_EQ(kv.value_size(), 0);
}

TEST(KeyValueTest, HappyPathReadKNullV) {

  nmq::KeyValueHeader sourse_buffer[] = {-1, sizeof(nmq::KeyValueHeader), 0};

  auto kv = nmq::KeyValue::read((char *)sourse_buffer, sizeof(sourse_buffer));

  EXPECT_EQ(kv.has_key(), false);
  EXPECT_EQ(kv.key_size(), 0);
  EXPECT_EQ(kv.has_value(), true);
  EXPECT_EQ(kv.value_size(), 4);
}

TEST(KeyValueTest, HappyPathReadKV) {

  nmq::KeyValueHeader sourse_buffer[] = {sizeof(nmq::KeyValueHeader),
                                         sizeof(nmq::KeyValueHeader), 1, 1};

  auto kv = nmq::KeyValue::read((char *)sourse_buffer, sizeof(sourse_buffer));

  EXPECT_EQ(kv.has_key(), true);
  EXPECT_EQ(kv.key_size(), 4);
  EXPECT_EQ(kv.has_value(), true);
  EXPECT_EQ(kv.value_size(), 4);
}

TEST(KeyValueTest, SadPathReadK) {
  nmq::KeyValueHeader sourse_buffer[1];

  EXPECT_THROW(
      nmq::KeyValue::read((char *)sourse_buffer, sizeof(sourse_buffer)),
      nmq::ActualSizeLessThanMinSize);
}

TEST(KeyValueTest, SadPathReadNullptr) {
  EXPECT_THROW(nmq::KeyValue::read(nullptr, 0), std::invalid_argument);
}

TEST(KeyValueTest, SadPathReadSmallThanKV) {
  nmq::KeyValueHeader sourse_buffer[1];

  EXPECT_THROW(
      nmq::KeyValue::read((char *)sourse_buffer, sizeof(sourse_buffer)),
      nmq::ActualSizeLessThanMinSize);
}

TEST(KeyValueTest, SadPathReadKBiggerVOk) {
  nmq::KeyValueHeader sourse_buffer[] = {100, 0};

  EXPECT_THROW(
      nmq::KeyValue::read((char *)sourse_buffer, sizeof(sourse_buffer)),
      nmq::ActualSizeLessThanExpectedSize);
}

TEST(KeyValueTest, SadPathReadKOkVBigger) {
  nmq::KeyValueHeader sourse_buffer[] = {0, 100};

  EXPECT_THROW(
      nmq::KeyValue::read((char *)sourse_buffer, sizeof(sourse_buffer)),
      nmq::ActualSizeLessThanExpectedSize);
}

//-------------------------------------------------------------------------------

TEST(KeyValueTest, HappyPathWriteEmptyKV) {
  nmq::KeyValueHeader source_buffer[] = {0, 0};

  auto source_kv =
      nmq::KeyValue::read((char *)source_buffer, sizeof(source_buffer));

  char target_buffer[sizeof(source_buffer)];

  source_kv.write(target_buffer, sizeof(target_buffer));

  auto target_kv =
      nmq::KeyValue::read((char *)target_buffer, sizeof(target_buffer));

  EXPECT_EQ(source_kv.has_key(), target_kv.has_key());
  EXPECT_EQ(source_kv.key_size(), target_kv.key_size());
  EXPECT_EQ(source_kv.has_value(), target_kv.has_value());
  EXPECT_EQ(source_kv.value_size(), target_kv.value_size());
}

TEST(KeyValueTest, HappyPathWriteKNullVNull) {
  nmq::KeyValueHeader source_buffer[] = {-1, -1};

  auto source_kv =
      nmq::KeyValue::read((char *)source_buffer, sizeof(source_buffer));

  char target_buffer[sizeof(source_buffer)];

  source_kv.write(target_buffer, sizeof(target_buffer));

  auto target_kv =
      nmq::KeyValue::read((char *)target_buffer, sizeof(target_buffer));

  EXPECT_EQ(source_kv.has_key(), target_kv.has_key());
  EXPECT_EQ(source_kv.key_size(), target_kv.key_size());
  EXPECT_EQ(source_kv.has_value(), target_kv.has_value());
  EXPECT_EQ(source_kv.value_size(), target_kv.value_size());
}

TEST(KeyValueTest, HappyPathWriteKVNull) {
  nmq::KeyValueHeader source_buffer[] = {sizeof(nmq::KeyValueHeader), 0, -1};

  auto source_kv =
      nmq::KeyValue::read((char *)source_buffer, sizeof(source_buffer));

  char target_buffer[sizeof(source_buffer)];

  source_kv.write(target_buffer, sizeof(target_buffer));

  auto target_kv =
      nmq::KeyValue::read((char *)target_buffer, sizeof(target_buffer));

  EXPECT_EQ(source_kv.has_key(), target_kv.has_key());
  EXPECT_EQ(source_kv.key_size(), target_kv.key_size());
  EXPECT_EQ(source_kv.has_value(), target_kv.has_value());
  EXPECT_EQ(source_kv.value_size(), target_kv.value_size());
}

TEST(KeyValueTest, HappyPathWriteKNullV) {
  nmq::KeyValueHeader source_buffer[] = {-1, sizeof(nmq::KeyValueHeader), 0};

  auto source_kv =
      nmq::KeyValue::read((char *)source_buffer, sizeof(source_buffer));

  char target_buffer[sizeof(source_buffer)];

  source_kv.write(target_buffer, sizeof(target_buffer));

  auto target_kv =
      nmq::KeyValue::read((char *)target_buffer, sizeof(target_buffer));

  EXPECT_EQ(source_kv.has_key(), target_kv.has_key());
  EXPECT_EQ(source_kv.key_size(), target_kv.key_size());
  EXPECT_EQ(source_kv.has_value(), target_kv.has_value());
  EXPECT_EQ(source_kv.value_size(), target_kv.value_size());
}

TEST(KeyValueTest, HappyPathWriteKV) {
  nmq::KeyValueHeader source_buffer[] = {sizeof(nmq::KeyValueHeader),
                                         sizeof(nmq::KeyValueHeader), 1, 1};

  auto source_kv =
      nmq::KeyValue::read((char *)source_buffer, sizeof(source_buffer));

  char target_buffer[sizeof(source_buffer)];

  source_kv.write(target_buffer, sizeof(target_buffer));

  auto target_kv =
      nmq::KeyValue::read((char *)target_buffer, sizeof(target_buffer));

  EXPECT_EQ(source_kv.has_key(), target_kv.has_key());
  EXPECT_EQ(source_kv.key_size(), target_kv.key_size());
  EXPECT_EQ(source_kv.has_value(), target_kv.has_value());
  EXPECT_EQ(source_kv.value_size(), target_kv.value_size());
}

TEST(KeyValueTest, SadPathWriteNullBuffer) {
  nmq::KeyValueHeader source_buffer[] = {sizeof(nmq::KeyValueHeader),
                                         sizeof(nmq::KeyValueHeader), 1, 1};

  auto source_kv =
      nmq::KeyValue::read((char *)source_buffer, sizeof(source_buffer));

  EXPECT_THROW(source_kv.write(nullptr, sizeof(source_buffer)),
               std::invalid_argument);
}

TEST(KeyValueTest, SadPathWriteSmallerThanNecesseryBuffer) {
  nmq::KeyValueHeader source_buffer[] = {sizeof(nmq::KeyValueHeader),
                                         sizeof(nmq::KeyValueHeader), 1, 1};

  auto source_kv =
      nmq::KeyValue::read((char *)source_buffer, sizeof(source_buffer));

  EXPECT_THROW(
      source_kv.write((char *)source_buffer, sizeof(source_buffer) - 1),
      nmq::ActualSizeLessThanExpectedSize);
}

TEST(KeyValueTest, SadPathWriteSmallerThanKVHeaderBuffer) {
  nmq::KeyValueHeader source_buffer[] = {sizeof(nmq::KeyValueHeader),
                                         sizeof(nmq::KeyValueHeader), 1, 1};

  auto source_kv =
      nmq::KeyValue::read((char *)source_buffer, sizeof(source_buffer));

  EXPECT_THROW(
      source_kv.write((char *)source_buffer, sizeof(source_buffer) - 1),
      nmq::ActualSizeLessThanExpectedSize);
}
