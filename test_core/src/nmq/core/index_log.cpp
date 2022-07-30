#include <gtest/gtest.h>

#include <filesystem>
#include <nmq/core/index_log.h>
#include <spdlog/spdlog.h>

TEST(IndexLogTest, BasicHappyPath) {

  std::string tmp_directory =
      std::filesystem::temp_directory_path() /
      ("testPushBackHappyPath" + std::to_string(std::time(nullptr)));

  nmq::IndexLog log =
      nmq::IndexLog(tmp_directory, (sizeof(nmq::IndexChunk) * 7) + 1);
  for (auto i = 0; i < 90; i = i + 1) {
    nmq::IndexChunk c = {._offset = i, ._position = i, ._size = 9};
    log.push_back(c);
  }

  auto r1 = log.load(0, 10);
  spdlog::info("r1->size() == {}", r1->size());
  auto r2 = log.load(20, 10);
  spdlog::info("r2->size() == {}", r2->size());
  auto r3 = log.load(90, 10);
  spdlog::info("r3->size() == {}", r3->size());
}