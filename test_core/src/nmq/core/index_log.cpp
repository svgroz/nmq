#include <gtest/gtest.h>

#include <filesystem>
#include <nmq/core/index_log.h>

TEST(IndexLogTest, BasicHappyPath) {

  std::string tmp_directory =
      std::filesystem::temp_directory_path() /
      ("testPushBackHappyPath" + std::to_string(std::time(nullptr)));

  nmq::IndexLog log = nmq::IndexLog(tmp_directory);
  for (auto i = 0; i < 99; i = i + 3) {
    nmq::IndexChunk c = {._offset = i, ._position = i, ._size = 9};
    log.push_back(c);
  }

  auto r1 =log.load(90, 10);
  auto r2 =log.load(0, 10);
  auto r3 =log.load(22, 10);

}