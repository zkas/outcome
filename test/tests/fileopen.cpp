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

#define _CRT_SECURE_NO_WARNINGS

#include "../../include/boost/outcome/outcome.hpp"
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_monitor.hpp>

#ifndef BOOST_NO_EXCEPTIONS

#ifdef _MSC_VER
#define BOOST_OUTCOME_POSIX_OPEN ::_open
#include <io.h>
#else
#define BOOST_OUTCOME_POSIX_OPEN ::open
#include <fcntl.h>
#endif

BOOST_OUTCOME_AUTO_TEST_CASE(works_outcome_fileopen, "Tests that the outcome semantically represents opening a file")
{
  using namespace BOOST_OUTCOME_V2_NAMESPACE;

  //! [file_open_example]
  auto openfile = [](std::string path) noexcept->outcome<int>
  {
    int fd;
    do
    {
      fd = BOOST_OUTCOME_POSIX_OPEN(path.c_str(), 0);  // NOLINT
    } while(-1 == fd && EINTR == errno);
    try
    {
      if(-1 == fd)
      {
        int code = errno;
        // If a temporary failure, this is an expected unexpected outcome
        if(EBUSY == code || EISDIR == code || ELOOP == code || ENOENT == code || ENOTDIR == code || EPERM == code || EACCES == code)
        {
          return std::error_code(code, std::generic_category());
        }
        // If a non-temporary failure, this is an unexpected outcome
        return std::make_exception_ptr(std::system_error(code, std::generic_category(), strerror(code)));
      }
      return fd;
    }
    catch(...)
    {
      // Any exception thrown is truly unexpected
      return std::current_exception();
    }
  };
  auto a = openfile("shouldneverexistnotever");
  BOOST_CHECK(!a);
  BOOST_CHECK(!a.has_value());
  BOOST_CHECK(!a.has_exception());
  BOOST_CHECK(a.has_error());
  BOOST_CHECK(a.error() == std::error_code(ENOENT, std::generic_category()));
  //! [file_open_example]
}
#endif
