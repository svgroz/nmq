#include <gtest/gtest.h>

#include <filesystem>
#include <nmq/core/index_log.h>
#include <spdlog/spdlog.h>

auto t(nmq::IndexLog &log, int offset, int count) {
  auto r = log.load(offset, count);
  spdlog::info("r->size() == {}", r->size());
  for (auto &i : *r) {
    spdlog::info("offset {}", i._offset);
  }
}

TEST(IndexLogTest, BasicHappyPath) {

  std::string tmp_directory =
      std::filesystem::temp_directory_path() /
      ("testPushBackHappyPath" + std::to_string(std::time(nullptr)));

  nmq::IndexLog log =
      nmq::IndexLog(tmp_directory, (sizeof(nmq::IndexChunk) * 3) + 1);
  for (auto i = 0; i < 120; i = i + 3) {
    nmq::IndexChunk c = {._offset = i, ._position = i, ._size = 9};
    log.push_back(c);
  }

  t(log, 0, 10);
  t(log, 61, 10);
  t(log, 119, 10);
}