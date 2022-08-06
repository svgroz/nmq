#pragma once

#include "limits.h"
#include <climits>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <limits>
#include <memory>
#include <vector>

#include <boost/compute/detail/lru_cache.hpp>

#include <nmq/core/buffer.h>
#include <nmq/core/index_chunk.h>
#include <nmq/core/lru.h>
#include <nmq/core/types.h>

namespace nmq {

class IndexLog final {
private:
  using BufferContext = struct BufferContext {
    std::int_fast64_t _page;
    std::int_fast64_t _chunks_on_page;
  };

  using PageBuffer = std::shared_ptr<Buffer<BufferContext>>;

  std::string _filename;
  std::fstream _file;
  const std::int_fast64_t _page_size;
  const std::int_fast64_t _chunks_on_page;
  const std::int_fast64_t _chunks_on_page_raw_size;
  const std::int_fast64_t _page_tile_size;
  LRU<std::int64_t, PageBuffer, 20> _page_cache;

  auto chunks_available(std::int_fast64_t file_size) noexcept
      -> std::int_fast64_t;
  auto load_page_buffer(std::int_fast64_t page_index,
                        std::int_fast64_t file_size) -> PageBuffer;
  auto upper_bound(message_offset_t offset, std::int_fast64_t file_size)
      -> std::int_fast64_t;

public:
  explicit IndexLog(std::string &filename, std::int_fast64_t page_size);
  IndexLog(IndexLog &&) = delete;
  virtual ~IndexLog() = default;
  auto push_back(IndexChunk index_chunk) -> void;
  auto load(message_offset_t offset, std::size_t count)
      -> std::unique_ptr<std::vector<IndexChunk>>;
};

} // namespace nmq