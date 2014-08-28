#include <iostream>
#include <vector>

#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/logical.hpp>
#include <boost/mpl/same_as.hpp>
#include <boost/utility/enable_if.hpp>

namespace test
{
  using namespace boost;

  struct FooPrint
  {
    template <typename U, typename V>
    static typename boost::enable_if<boost::is_same<U, V> >::type
    print ()
    {
      std::cout << "Same! :-)" << std::endl;
    }


    template <typename U, typename V>
    static typename boost::disable_if<boost::is_same<U, V> >::type
    print ()
    {
      std::cout << "Not same! :-(" << std::endl;
    }
  };

  template <typename U, typename V>
  class Foo
  {
  public:
    Foo () {}

    void print ()
    {
      FooPrint::print<U,V> ();
    }

  };

} // end of namespace test.

int main()
{
  using namespace test;

  Foo<int,int> f;
  f.print ();

  Foo<int,double> g;
  g.print ();

  return 0;
}
