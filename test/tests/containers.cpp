/* Unit testing for outcomes
(C) 2013-2017 Niall Douglas <http://www.nedproductions.biz/> (149 commits)


Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#include "../../include/boost/outcome/outcome.hpp"
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_monitor.hpp>

BOOST_OUTCOME_AUTO_TEST_CASE(works_outcome_containers, "Tests that outcome works as intended inside containers")
{
  using namespace BOOST_OUTCOME_V2_NAMESPACE;
  outcome<std::vector<int>> a(std::vector<int>{5, 6, 7, 8});
  BOOST_CHECK(a.has_value());
  BOOST_CHECK(a.value().size() == 4U);
  auto b(a);
  BOOST_CHECK(a.has_value());
  BOOST_CHECK(a.value().size() == 4U);
  BOOST_CHECK(b.has_value());
  BOOST_CHECK(b.value().size() == 4U);

  std::vector<outcome<std::vector<int>>> vect;
  vect.push_back(std::vector<int>{5, 6, 7, 8});
  vect.push_back(std::vector<int>{1, 2, 3, 4});
  BOOST_REQUIRE(vect.size() == 2U);
  BOOST_CHECK(vect[0].has_value());
  BOOST_CHECK(vect[1].has_value());
  BOOST_CHECK(vect[0].value().size() == 4U);
  BOOST_CHECK(vect[1].value().size() == 4U);
  BOOST_CHECK(vect[0].value().front() == 5);
  BOOST_CHECK(vect[0].value().back() == 8);
  BOOST_CHECK(vect[1].value().front() == 1);
  BOOST_CHECK(vect[1].value().back() == 4);
}
