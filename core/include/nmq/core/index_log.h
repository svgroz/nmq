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
  virtual ~IndexLog() = default;
  auto push_back(IndexChunk &index_chunk) -> void;
  auto load(message_offset_t offset, std::size_t count)
      -> std::unique_ptr<std::vector<IndexChunk>>;
};

} // namespace nmq