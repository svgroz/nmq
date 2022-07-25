#include <gtest/gtest.h>

#include <filesystem>
#include <nmq/core/index_log.h>

TEST(IndexLogTest, BasicHappyPath) {

  std::string tmp_directory =
      std::filesystem::temp_directory_path() /
      ("testPushBackHappyPath" + std::to_string(std::time(nullptr)));

  nmq::IndexLog log = nmq::IndexLog(tmp_directory);
  for (auto i = 0; i < 10; i++) {
    nmq::IndexChunk c(i, 0, 0);
    log.push_back(c);
  }

  log.load(0, 10);
}