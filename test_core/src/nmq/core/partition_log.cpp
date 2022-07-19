#include <gtest/gtest.h>

#include <ctime>
#include <nmq/core/partition_log.h>

TEST(PartitionLog, PushBackHappyPath) {
  using namespace nmq;

  PartitionLog partition_log("/tmp/testPushBackHappyPath" +
                             std::to_string(std::time(nullptr)));
  std::string key_value = "hhhheeeellloooooooooo!!!";
  std::vector<char> key_data(key_value.begin(), key_value.end());
  std::string value_value = "WWWWwwwwwoooooooooorld221145435";
  std::vector<char> value_data(value_value.begin(), value_value.end());
  KeyValue kv = {key_data, true, value_data, true};
  std::vector<KeyValue> headers = {};
  auto message = Message(std::move(kv), std::move(headers));
  partition_log.push_back(message);
}