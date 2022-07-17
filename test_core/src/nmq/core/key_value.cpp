#include <gtest/gtest.h>

#include <cstring>

#include <nmq/core/exceptions.h>
#include <nmq/core/key_value.h>

TEST(KeyValueTest, HappyPathReadKEmptyVEmpty) {

  nmq::key_value::key_value_t source_buffer[] = {0, 0};

  auto kv = nmq::key_value::KeyValue::read((char *)source_buffer, sizeof(source_buffer));

  EXPECT_EQ(kv.has_key(), true);
  EXPECT_EQ(kv.key_size(), 0);
  EXPECT_EQ(kv.has_value(), true);
  EXPECT_EQ(kv.value_size(), 0);
}

TEST(KeyValueTest, HappyPathReadKNullVNull) {

  nmq::key_value::key_value_t source_buffer[] = {-1, -1};

  auto kv = nmq::key_value::KeyValue::read((char *)source_buffer, sizeof(source_buffer));

  EXPECT_EQ(kv.has_key(), false);
  EXPECT_EQ(kv.key_size(), 0);
  EXPECT_EQ(kv.has_value(), false);
  EXPECT_EQ(kv.value_size(), 0);
}

TEST(KeyValueTest, HappyPathReadKVNull) {

  nmq::key_value::key_value_t source_buffer[] = {sizeof(nmq::key_value::key_value_t), -1, -1};

  auto kv = nmq::key_value::KeyValue::read((char *)source_buffer, sizeof(source_buffer));

  EXPECT_EQ(kv.has_key(), true);
  EXPECT_EQ(kv.key_size(), 4);
  EXPECT_EQ(kv.has_value(), false);
  EXPECT_EQ(kv.value_size(), 0);
}

TEST(KeyValueTest, HappyPathReadKNullV) {

  nmq::key_value::key_value_t source_buffer[] = {-1, sizeof(nmq::key_value::key_value_t), 0};

  auto kv = nmq::key_value::KeyValue::read((char *)source_buffer, sizeof(source_buffer));

  EXPECT_EQ(kv.has_key(), false);
  EXPECT_EQ(kv.key_size(), 0);
  EXPECT_EQ(kv.has_value(), true);
  EXPECT_EQ(kv.value_size(), 4);
}

TEST(KeyValueTest, HappyPathReadKNullVLarge) {
  std::size_t source_buffer_size = sizeof(char) * 1024 * 1024 * 64; // 64mb
  std::size_t value_size =
      source_buffer_size - (sizeof(nmq::key_value::key_value_t) * 2);
  char *raw_source_buffer = new char[source_buffer_size];
  ((nmq::key_value::key_value_t *)raw_source_buffer)[0] = 0;
  ((nmq::key_value::key_value_t *)raw_source_buffer)[1] = value_size;

  auto kv = nmq::key_value::KeyValue::read(raw_source_buffer, source_buffer_size);

  delete[] raw_source_buffer;

  EXPECT_EQ(kv.has_key(), true);
  EXPECT_EQ(kv.key_size(), 0);
  EXPECT_EQ(kv.has_value(), true);
  EXPECT_EQ(kv.value_size(), value_size);
  EXPECT_EQ(kv.size(), source_buffer_size);
}

TEST(KeyValueTest, HappyPathReadKV) {

  nmq::key_value::key_value_t source_buffer[] = {sizeof(nmq::key_value::key_value_t),
                                         sizeof(nmq::key_value::key_value_t), 1, 1};

  auto kv = nmq::key_value::KeyValue::read((char *)source_buffer, sizeof(source_buffer));

  EXPECT_EQ(kv.has_key(), true);
  EXPECT_EQ(kv.key_size(), 4);
  EXPECT_EQ(kv.has_value(), true);
  EXPECT_EQ(kv.value_size(), 4);
}

TEST(KeyValueTest, SadPathReadK) {
  nmq::key_value::key_value_t source_buffer[1];

  EXPECT_THROW(
      nmq::key_value::KeyValue::read((char *)source_buffer, sizeof(source_buffer)),
      nmq::exception::ActualLessThanExpected);
}

TEST(KeyValueTest, SadPathReadNullptr) {
  EXPECT_THROW(nmq::key_value::KeyValue::read(nullptr, 0),
               nmq::exception::NullptrArgumentException);
}

TEST(KeyValueTest, SadPathReadSmallThanKV) {
  nmq::key_value::key_value_t source_buffer[1];

  EXPECT_THROW(
      nmq::key_value::KeyValue::read((char *)source_buffer, sizeof(source_buffer)),
      nmq::exception::ActualLessThanExpected);
}

TEST(KeyValueTest, SadPathReadKBiggerVOk) {
  nmq::key_value::key_value_t source_buffer[] = {100, 0};

  EXPECT_THROW(
      nmq::key_value::KeyValue::read((char *)source_buffer, sizeof(source_buffer)),
      nmq::exception::ActualLessThanExpected);
}

TEST(KeyValueTest, SadPathReadKOkVBigger) {
  nmq::key_value::key_value_t source_buffer[] = {0, 100};

  EXPECT_THROW(
      nmq::key_value::KeyValue::read((char *)source_buffer, sizeof(source_buffer)),
      nmq::exception::ActualLessThanExpected);
}

//-------------------------------------------------------------------------------

TEST(KeyValueTest, HappyPathWriteEmptyKV) {
  nmq::key_value::key_value_t source_buffer[] = {0, 0};

  auto source_kv =
      nmq::key_value::KeyValue::read((char *)source_buffer, sizeof(source_buffer));

  char target_buffer[sizeof(source_buffer)];

  source_kv.write(target_buffer, sizeof(target_buffer));

  auto target_kv =
      nmq::key_value::KeyValue::read((char *)target_buffer, sizeof(target_buffer));

  EXPECT_EQ(source_kv.has_key(), target_kv.has_key());
  EXPECT_EQ(source_kv.key_size(), target_kv.key_size());
  EXPECT_EQ(source_kv.has_value(), target_kv.has_value());
  EXPECT_EQ(source_kv.value_size(), target_kv.value_size());
}

TEST(KeyValueTest, HappyPathWriteKNullVNull) {
  nmq::key_value::key_value_t source_buffer[] = {-1, -1};

  auto source_kv =
      nmq::key_value::KeyValue::read((char *)source_buffer, sizeof(source_buffer));

  char target_buffer[sizeof(source_buffer)];

  source_kv.write(target_buffer, sizeof(target_buffer));

  auto target_kv =
      nmq::key_value::KeyValue::read((char *)target_buffer, sizeof(target_buffer));

  EXPECT_EQ(source_kv.has_key(), target_kv.has_key());
  EXPECT_EQ(source_kv.key_size(), target_kv.key_size());
  EXPECT_EQ(source_kv.has_value(), target_kv.has_value());
  EXPECT_EQ(source_kv.value_size(), target_kv.value_size());
}

TEST(KeyValueTest, HappyPathWriteKVNull) {
  nmq::key_value::key_value_t source_buffer[] = {sizeof(nmq::key_value::key_value_t), 0, -1};

  auto source_kv =
      nmq::key_value::KeyValue::read((char *)source_buffer, sizeof(source_buffer));

  char target_buffer[sizeof(source_buffer)];

  source_kv.write(target_buffer, sizeof(target_buffer));

  auto target_kv =
      nmq::key_value::KeyValue::read((char *)target_buffer, sizeof(target_buffer));

  EXPECT_EQ(source_kv.has_key(), target_kv.has_key());
  EXPECT_EQ(source_kv.key_size(), target_kv.key_size());
  EXPECT_EQ(source_kv.has_value(), target_kv.has_value());
  EXPECT_EQ(source_kv.value_size(), target_kv.value_size());
}

TEST(KeyValueTest, HappyPathWriteKNullV) {
  nmq::key_value::key_value_t source_buffer[] = {-1, sizeof(nmq::key_value::key_value_t), 0};

  auto source_kv =
      nmq::key_value::KeyValue::read((char *)source_buffer, sizeof(source_buffer));

  char target_buffer[sizeof(source_buffer)];

  source_kv.write(target_buffer, sizeof(target_buffer));

  auto target_kv =
      nmq::key_value::KeyValue::read((char *)target_buffer, sizeof(target_buffer));

  EXPECT_EQ(source_kv.has_key(), target_kv.has_key());
  EXPECT_EQ(source_kv.key_size(), target_kv.key_size());
  EXPECT_EQ(source_kv.has_value(), target_kv.has_value());
  EXPECT_EQ(source_kv.value_size(), target_kv.value_size());
}

TEST(KeyValueTest, HappyPathWriteKV) {
  nmq::key_value::key_value_t source_buffer[] = {sizeof(nmq::key_value::key_value_t),
                                         sizeof(nmq::key_value::key_value_t), 1, 1};

  auto source_kv =
      nmq::key_value::KeyValue::read((char *)source_buffer, sizeof(source_buffer));

  char target_buffer[sizeof(source_buffer)];

  source_kv.write(target_buffer, sizeof(target_buffer));

  auto target_kv =
      nmq::key_value::KeyValue::read((char *)target_buffer, sizeof(target_buffer));

  EXPECT_EQ(source_kv.has_key(), target_kv.has_key());
  EXPECT_EQ(source_kv.key_size(), target_kv.key_size());
  EXPECT_EQ(source_kv.has_value(), target_kv.has_value());
  EXPECT_EQ(source_kv.value_size(), target_kv.value_size());
}

TEST(KeyValueTest, SadPathWriteNullBuffer) {
  nmq::key_value::key_value_t source_buffer[] = {sizeof(nmq::key_value::key_value_t),
                                         sizeof(nmq::key_value::key_value_t), 1, 1};

  auto source_kv =
      nmq::key_value::KeyValue::read((char *)source_buffer, sizeof(source_buffer));

  EXPECT_THROW(source_kv.write(nullptr, sizeof(source_buffer)),
               nmq::exception::NullptrArgumentException);
}

TEST(KeyValueTest, SadPathWriteSmallerThanNecesseryBuffer) {
  nmq::key_value::key_value_t source_buffer[] = {sizeof(nmq::key_value::key_value_t),
                                         sizeof(nmq::key_value::key_value_t), 1, 1};

  auto source_kv =
      nmq::key_value::KeyValue::read((char *)source_buffer, sizeof(source_buffer));

  EXPECT_THROW(
      source_kv.write((char *)source_buffer, sizeof(source_buffer) - 1),
      nmq::exception::ActualLessThanExpected);
}

TEST(KeyValueTest, SadPathWriteSmallerThanKVHeaderBuffer) {
  nmq::key_value::key_value_t source_buffer[] = {sizeof(nmq::key_value::key_value_t),
                                         sizeof(nmq::key_value::key_value_t), 1, 1};

  auto source_kv =
      nmq::key_value::KeyValue::read((char *)source_buffer, sizeof(source_buffer));

  EXPECT_THROW(
      source_kv.write((char *)source_buffer, sizeof(source_buffer) - 1),
      nmq::exception::ActualLessThanExpected);
}
