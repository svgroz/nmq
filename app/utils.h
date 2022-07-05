#pragma once

#include <mutex>
#include <string>
#include <fstream>

namespace nmq {
void file_is_open_check(std::fstream &file, const std::string &filename);

void close_file_gratefully(std::fstream &file, std::mutex &file_mutex,
                           const std::string &filename);
}