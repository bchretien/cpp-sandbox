#include <iostream>

#include <boost/mpl/assert.hpp>
#include <boost/mpl/logical.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/mpl/fold.hpp>

namespace test
{
  using namespace boost;

  class F0
  {
  public:
    F0 () {}
  };

  class F1
  {
  public:
    F1 () {}
  };

  class C0 : public F0
  {
  public:
    C0 () : F0 () {}
  };

  class C1 : public C0
  {
  public:
    C1 () : C0 () {}
  };

  class G
  {
  public:
    G () {}
  };

  template <typename F, typename CLIST>
  class Foo
  {
    // Check that F derives from F0 or F1.
    BOOST_MPL_ASSERT((mpl::or_<is_base_of<F0, F>,
                               is_base_of<F1, F> >));

    // Check that all the elements of CLIST derive from F0 or F1.
    BOOST_MPL_ASSERT((mpl::fold<CLIST,
                                mpl::bool_<true>,
                                mpl::if_<mpl::or_<is_base_of<F0, mpl::_2>,
                                                  is_base_of<F1, mpl::_2> >,
                                         mpl::_1, mpl::bool_<false> > >));

  public:
    Foo () {}

    void bar ()
    {
      std::cout << "bar" << std::endl;
    }
  };

} // end of namespace test.

int main()
{
  using namespace test;

  // Valid types.
  typedef F0 F;
  typedef boost::mpl::vector<C0,C1> CLIST;

  Foo<F, CLIST> foo;
  foo.bar ();

  // Wrong types.
  typedef G fail_F;
  typedef boost::mpl::vector<C0,G> fail_CLIST;

  // With wrong types, compilation fails:
  //Foo<fail_F, CLIST> fail_foo1;
  //fail_foo1.bar ();
  //Foo<F, fail_CLIST> fail_foo2;
  //fail_foo2.bar ();

  return 0;
}
