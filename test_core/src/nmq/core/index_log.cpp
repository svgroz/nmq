#include <gtest/gtest.h>

#include <filesystem>
#include <nmq/core/index_log.h>

TEST(IndexLogTest, BasicHappyPath) {

  auto tmp_directory =
      std::filesystem::temp_directory_path() /
      ("testPushBackHappyPath" + std::to_string(std::time(nullptr)));

  std::string full_path = tmp_directory.u8string();
  nmq::IndexLog log(full_path, 0);

  nmq::IndexChunk test_data_1{8, 0, 0};
  auto test_result_1 = log.find(test_data_1._offset);

  nmq::IndexChunk test_data_2{554, 54, 22};
  log.set(test_data_2);
  auto test_result_2 = log.find(test_data_2._offset);

  ASSERT_FALSE(test_result_1.has_value());
  
  ASSERT_TRUE(test_result_2.has_value());
  ASSERT_EQ(test_result_2->_offset, test_data_2._offset);
  ASSERT_EQ(test_result_2->_position, test_data_2._position);
  ASSERT_EQ(test_result_2->_message_size, test_data_2._message_size);
}