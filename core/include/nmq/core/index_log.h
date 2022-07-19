#pragma once

#include <fstream>
#include <memory>
#include <nmq/core/index_chunk.h>
#include <nmq/core/types.h>
#include <optional>
#include <vector>

namespace nmq {

class IndexLog final {
private:
  std::string _filename;
  std::fstream _file;

public:
  explicit IndexLog(std::string &filename);
  IndexLog(IndexLog &&) = delete;
  virtual ~IndexLog();
  auto push_back(IndexChunk &index_chunk) -> void;
  auto find(message_offset_t offset) -> std::optional<IndexChunk>;
};

} // namespace nmq