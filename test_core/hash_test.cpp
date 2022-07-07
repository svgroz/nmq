#include <iostream>
#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>
#include <nmq/core/hash.h>

BOOST_AUTO_TEST_CASE(hash_code_eq) {

  auto x = nmq::hashCode("foo", 3);

  BOOST_CHECK(x != 0);
}
