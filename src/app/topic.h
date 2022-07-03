#pragma once

#include <fstream>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

namespace nmq {
class FileLock {
private:
  FileLock(const FileLock &) = delete;

public:
  FileLock(std::string filename);
  virtual ~FileLock();
  const std::string _filename;
  std::ifstream _file;
  std::mutex _mutex;
};

class Topic {
private:
  Topic(const Topic &) = delete;
  std::vector<std::unique_ptr<FileLock>> _topics_lock_file;

public:
  Topic(std::string name, uint64_t partitions);
  virtual ~Topic();
};

}; // namespace nmq