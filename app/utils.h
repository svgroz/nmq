#pragma once

#include <mutex>
#include <string>
#include <fstream>

namespace nmq {
void close_file_gratefully(std::fstream &file, std::mutex &file_mutex,
                           const std::string &filename);
}