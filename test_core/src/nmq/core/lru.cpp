#include <cstddef>
#include <fstream>
#include <gtest/gtest.h>

#include <memory>
#include <nmq/core/lru.h>

TEST(LRUTest, BasicHappyPath) {
  auto cache = std::make_shared<nmq::LRU<int, std::string, 3>>();

  cache->insert(1, "a");
  cache->insert(2, "b");
  cache->insert(3, "c");
  cache->insert(4, "d");
  cache->insert(5, "e");
  cache->insert(1, "f");
  

  ASSERT_TRUE(cache->get(1).has_value());
  ASSERT_TRUE(!cache->get(2).has_value());
  ASSERT_TRUE(!cache->get(3).has_value());
  ASSERT_TRUE(cache->get(4).has_value());
  ASSERT_TRUE(cache->get(5).has_value());
  
}