#include <iostream>
#include <vector>

#include <boost/mpl/assert.hpp>
#include <boost/mpl/count_if.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/greater.hpp>
#include <boost/mpl/logical.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/variant.hpp>
#include <boost/variant/apply_visitor.hpp>

namespace test
{
  using namespace boost;

  class Function
  {
  public:
    Function () {}

    virtual void print () const
    {
      std::cout << "Function";
    }
  };

  struct printFunction : public boost::static_visitor<void>
  {
    printFunction () {}

    template <typename U>
    void operator () (const U& func) const
    {
      func.print ();
    }
  };

  class F0 : public Function
  {
  public:
    F0 () : Function () {}

    virtual void print () const
    {
      std::cout << "F0";
    }
  };

  class F1 : public Function
  {
  public:
    F1 () : Function () {}

    virtual void print () const
    {
      std::cout << "F1";
    }
  };

  class C0 : public F0
  {
  public:
    C0 () : F0 () {}

    virtual void print () const
    {
      std::cout << "C0";
    }
  };

  class C1 : public C0
  {
  public:
    C1 () : C0 () {}

    virtual void print () const
    {
      std::cout << "C1";
    }
  };

  class G : public Function
  {
  public:
    G () : Function () {}

    virtual void print () const
    {
      std::cout << "G";
    }
  };

  // Whether Sequence contains a base of Type.
  template<typename Sequence, typename Type>
  struct contains_base_of
  : boost::mpl::greater<
      boost::mpl::int_<boost::mpl::count_if<Sequence,
                                            boost::is_base_of<boost::mpl::_, Type>
      >::value>,
      boost::mpl::int_<0> >
  {};

  template <typename F, typename FLIST>
  class Foo
  {
    // Check that F derives from F0 or F1.
    BOOST_MPL_ASSERT_MSG((mpl::or_<is_base_of<F0, F>,
                                  is_base_of<F1, F> >::value),
                         F0_OR_F1_EXPECTED_FOR_F, (F));

    // Check that all the elements of FLIST derive from F0 or F1.
    BOOST_MPL_ASSERT_MSG((mpl::fold<FLIST,
                                    mpl::bool_<true>,
                                    mpl::if_<mpl::or_<is_base_of<F0, mpl::_2>,
                                                      is_base_of<F1, mpl::_2> >,
                                             mpl::_1, mpl::bool_<false> > >::type::value),
                         F0_OR_F1_EXPECTED_FOR_FLIST, (FLIST));

  public:
    template <typename F_, typename FLIST_>
    friend class Foo;

    typedef typename boost::make_variant_over<FLIST>::type function_t;
    typedef std::vector<function_t> functions_t;

    explicit Foo ()
      : f_ (),
        functions_ ()
    {}

    template <typename F_, typename FLIST_>
    explicit Foo (const Foo<F_, FLIST_>& foo)
      : f_ (foo.f_),
        functions_ (foo.functions_.size ())
    {
      // Check that F derives from F0 or F1.
      BOOST_MPL_ASSERT_MSG((is_base_of<F, F_>::value),
                           SAME_F_TYPE_EXPECTED, (F, F_));

      // Check that FLIST_ is a subset of FLIST (i.e. all the functions
      // of FLIST_ are or derive from functions of FLIST).
      // The algorithm is as follows:
      // (0) outer_ok = true
      // (1) for c_ in FLIST_:
      // (2)   inner_ok = false
      // (3)   for c in FLIST:
      // (4)     if c is_base_of c_:
      // (5)       inner_ok = true
      // (6)   if not inner_ok:
      // (7)     outer_ok = false
      // (8) return outer_ok
      using namespace boost;

      BOOST_MPL_ASSERT_MSG(
        (mpl::fold<FLIST_, // (1)
                   mpl::bool_<true>, // (0)
                   mpl::if_< // (6)
                     contains_base_of<FLIST,mpl::_2>, // (2,3,4,5)
                     mpl::_1,
                     mpl::bool_<false> // (7)
                   >
         >::type::value), // (8)
         INCOMPATIBLE_TYPES_IN_LIST, (FLIST_, FLIST));

      // Copy constraints.
      std::copy (foo.functions_.begin (), foo.functions_.end (),
                 functions_.begin ());
    }

    functions_t& functions ()
    {
      return functions_;
    }

    const functions_t& functions () const
    {
      return functions_;
    }

    void print () const
    {
      std::cout << "Foo" << std::endl;

      std::cout << "  f = ";
      f_.print ();
      std::cout << std::endl;

      std::cout << "  list = ";
      for (typename functions_t::const_iterator
           f  = functions_.begin ();
           f != functions_.end ();
           ++f)
      {
        boost::apply_visitor (printFunction (), *f);
        std::cout << " ";
      }

      std::cout << std::endl << std::endl;
    }

  private:

    F           f_;
    functions_t functions_;
  };

} // end of namespace test.

int main()
{
  using namespace test;

  /// FIRST: check that template parameters derive from some types.
  // Valid types.
  typedef C0 F;
  typedef boost::mpl::vector<C0,C1> FLIST;

  Foo<F, FLIST> foo;
  std::cout << "Foo<C0, vector<C0,C1>>" << std::endl;
  foo.functions ().push_back (C0 ());
  foo.functions ().push_back (C1 ());
  foo.print ();

  // Wrong types.
  //typedef G fail_F;
  //typedef boost::mpl::vector<F0,G> fail_FLIST;

  // With wrong types, compilation fails:
  //Foo<fail_F, FLIST> fail_foo1;
  //fail_foo1.print ();
  //Foo<F, fail_FLIST> fail_foo2;
  //fail_foo2.print ();

  /// SECOND: check that a vector of types is a subset of another vector.
  typedef F0 F_cp;
  typedef boost::mpl::vector<F0,F1> FLIST_cp;

  Foo<F_cp, FLIST> foo_cp1 (foo);
  std::cout << "Foo<F0, vector<C0,C1>>" << std::endl;
  foo_cp1.functions ().push_back (C0 ());
  foo_cp1.functions ().push_back (C1 ());
  foo_cp1.print ();

  Foo<F, FLIST_cp> foo_cp2 (foo);
  std::cout << "Foo<C0, vector<F0,F1>>" << std::endl;
  foo_cp2.functions ().push_back (C0 ());
  foo_cp2.functions ().push_back (C1 ());
  foo_cp2.print ();

  // With wrong constraint types, compilation fails:
  //typedef G fail_F_cp;
  //typedef boost::mpl::vector<C1> fail_FLIST_cp;
  //Foo<fail_F_cp, FLIST_cp> foo_cp3 (foo);
  //Foo<F_cp, fail_FLIST_cp> foo_cp4 (foo);

  return 0;
}
