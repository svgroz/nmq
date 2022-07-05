#include "utils.h"
#include <boost/log/trivial.hpp>
#include <fstream>

void nmq::file_is_open_check(std::fstream &file, const std::string &filename) {
  if (file.is_open()) {
    BOOST_LOG_TRIVIAL(debug) << "file is open: " << filename;
  } else {
    BOOST_LOG_TRIVIAL(error) << "could not open file: " << filename;
    throw "TODO"; // TODO
  }
};

void nmq::close_file_gratefully(std::fstream &file, std::mutex &file_mutex,
                                const std::string &filename) {
  try {
    std::lock_guard<std::mutex> guard(file_mutex);

    if (file.is_open()) {
      BOOST_LOG_TRIVIAL(debug) << "trying to close file: " << filename;
      file.close();
      if (file.is_open()) {
        BOOST_LOG_TRIVIAL(debug) << "could not close file: " << filename;
      } else {
        BOOST_LOG_TRIVIAL(debug) << "file successfully closed: " << filename;
      }
    } else {
      BOOST_LOG_TRIVIAL(warning) << "file is already closed: " << filename;
    }
    file.close();
  } catch (void *ex) {
    throw ex;
  }
}