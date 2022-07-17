#include <gtest/gtest.h>

#include <nmq/core/message.h>

TEST(MessageTest, HappyPathReadEmptyNull) {
  nmq::key_value::key_value_t sourse_buffer[] = {0, -1};

  auto message =
      nmq::message::Message::read((char *)sourse_buffer, sizeof(sourse_buffer));

  EXPECT_EQ(message.size(), sizeof(sourse_buffer));
}
