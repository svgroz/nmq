#include <gtest/gtest.h>

#include <nmq/core/message.h>

TEST(MessageTest, HappyPathReadEmptyNull) {
  nmq::KeyValueHeader sourse_buffer[] = {0, -1, -1, 0};

  nmq::Message message =
      nmq::Message::read((char *)sourse_buffer, sizeof(sourse_buffer));

  EXPECT_EQ(message.size(), sizeof(sourse_buffer));
}
