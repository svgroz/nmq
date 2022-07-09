#define BOOST_TEST_MODULE example
#include <boost/test/included/unit_test.hpp>

#include <nmq/core/message.h>

BOOST_AUTO_TEST_SUITE(message)

BOOST_AUTO_TEST_CASE(happy_path) {
  using namespace nmq;

  std::string s1 = "Hello ";
  std::vector<char> v1(s1.begin(), s1.end());

  std::string s2 = "World!!!";
  std::vector<char> v2(s2.begin(), s2.end());

  Message message1(v1, v2);
  std::shared_ptr<std::vector<char>> serialized = message1.serialize();
  auto message2 = Message::desserialize(*serialized);

  BOOST_TEST(message1.key() == message2->key());
  BOOST_TEST(message1.value() == message2->value());
}

BOOST_AUTO_TEST_CASE(desserialize_exceptions) {
  using namespace nmq;

  std::vector<char> source1{};
  std::vector<char> source2{'1', '2', '3', '4'};
  std::vector<char> source3{'1', '2', '3', '4', '1', '2', '3', '4'};
  std::vector<char> source4{'1', '2', '3', '4', '1', '2', '3', '4',
                            '1', '2', '3', '4', '1', '2', '3', '4'};

  BOOST_CHECK_THROW(Message::desserialize(source1), std::invalid_argument);
  BOOST_CHECK_THROW(Message::desserialize(source2), std::invalid_argument);
  BOOST_CHECK_THROW(Message::desserialize(source3), std::invalid_argument);
  BOOST_CHECK_THROW(Message::desserialize(source4), std::invalid_argument);
}

BOOST_AUTO_TEST_SUITE_END()
