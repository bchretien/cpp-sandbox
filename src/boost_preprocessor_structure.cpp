#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/pop_back.hpp>
#include <boost/preprocessor/seq/reverse.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/stringize.hpp>

#include <iostream>

#define SET_N(PARENT) \
  static const int N = PARENT::N

#define SET_M(PARENT) \
  static const int M = PARENT::M

/// \brief Forward float typedef for problem types based on policies.
/// \param PARENT parent type (ProblemTraits).
#define FWD_SET(PARENT) \
  SET_N(PARENT); \
  SET_M(PARENT)

// Version without semicolon
#define FWD_SET_PARTIAL(PARENT, ELEM) \
  BOOST_PP_CAT(SET_, ELEM) (PARENT)

// Version with semicolon
#define FWD_SET_PARTIAL_SC(R, PARENT, ELEM) \
  FWD_SET_PARTIAL(PARENT, ELEM);

// Do not add semicolon to last element to avoid warning: extra ‘;’
#define FWD_SET_LIST(PARENT, SEQ) \
  BOOST_PP_SEQ_FOR_EACH(FWD_SET_PARTIAL_SC, PARENT, BOOST_PP_SEQ_POP_BACK(SEQ)) \
  FWD_SET_PARTIAL(PARENT, BOOST_PP_SEQ_HEAD(BOOST_PP_SEQ_REVERSE(SEQ)))

#define SEQ_FOO (N)(M)
#define SEQ_BAR1 SEQ_FOO
#define SEQ_BAR2 (N)

struct Foo
{
  static const int N = 12;
  static const int M = 42;
};

struct Bar1
{
  FWD_SET_LIST(Foo, SEQ_BAR1);
};

struct Bar2
{
  FWD_SET_LIST(Foo, SEQ_BAR2);
};


#define PRINT_PARTIAL(R, PARENT, ELEM) \
  << "  " << BOOST_PP_STRINGIZE(ELEM) << " = " << PARENT::ELEM << endl

#define PRINT_TEST(C, SEQ)                     \
  cout << #C << ":" << endl                    \
  BOOST_PP_SEQ_FOR_EACH(PRINT_PARTIAL, C, SEQ) \
       << endl

int main()
{
  using namespace std;

  PRINT_TEST(Foo, SEQ_FOO);
  PRINT_TEST(Bar1, SEQ_BAR1);
  PRINT_TEST(Bar2, SEQ_BAR2);

  return 0;
}
