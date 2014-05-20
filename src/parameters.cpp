#include <iostream>

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/cat.hpp>

template <typename TT, int NN, int MM>
struct TestTraits
{
  typedef TT T;
  static const int N = NN;
  static const int M = MM;
};

// -------------------------------------------------------------------------- //
// --                     TEST MIXIN                                       -- //
// -------------------------------------------------------------------------- //

// Full typedefs
# define TEST_FWD_TYPEDEFS_PARENT(PARENT) \
  typedef PARENT parent_t;                \
  typedef typename parent_t::T T;         \
  static const int N = parent_t::N;       \
  static const int M = parent_t::M;       \
  struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n

// Partial typedefs (N and M are optional)
# define TEST_FWD_TYPEDEFS_PARENT_OPTIONAL(R,DATA,ELEM) \
  static const int ELEM = parent_t::ELEM;

# define TEST_FWD_TYPEDEFS_PARENT_PARTIAL(PARENT,SEQ)              \
  typedef PARENT parent_t;                                         \
  typedef typename parent_t::T T;                                  \
  BOOST_PP_SEQ_FOR_EACH(TEST_FWD_TYPEDEFS_PARENT_OPTIONAL, _, SEQ) \
  struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n

namespace test_mixin
{
  template <typename U>
  class Foo : public U
  {
  public:
    TEST_FWD_TYPEDEFS_PARENT (U);

    Foo () : t_ (12.)
    {
      std::cout << "Foo.t  = " << t_ << std::endl;
      std::cout << "Foo.N  = " << N << std::endl;
      std::cout << "Foo.M  = " << M << std::endl;
    }

  public:
    T t_;
  };

  template <typename U>
  class Bar : public U
  {
  public:
    TEST_FWD_TYPEDEFS_PARENT_PARTIAL (U,(N)(M));

    Bar (const Foo<U>& foo)
      : t1_ (foo.t_),
        t2_ (21.)
    {
      std::cout << "Bar.t1 = " << t1_ << std::endl;
      std::cout << "Bar.t2 = " << t2_ << std::endl;
      std::cout << "Bar.N  = " << N << std::endl;
      // Here M is accessible (with the proper typedef), even though it is
      // not needed.
      //std::cout << "Bar.M  = " << M << std::endl;
    }

  public:
    T t1_;
    T t2_;
  };
} // end of namespace test_mixin

// -------------------------------------------------------------------------- //
// --                        TEST POLICY                                   -- //
// -------------------------------------------------------------------------- //

// Full typedefs
# define TEST_FWD_TYPEDEFS_POLICY(PARENT)              \
  typedef typename Policies<PARENT>::TPolicy_t::T T;   \
  static const int N = Policies<PARENT>::NPolicy_t::N; \
  static const int M = Policies<PARENT>::MPolicy_t::M; \
  struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n

// Partial typedefs (N and M are optional)
# define TEST_FWD_TYPEDEFS_POLICY_OPTIONAL(R,PARENT,ELEM) \
  static const int ELEM = Policies<PARENT>::BOOST_PP_CAT(ELEM,Policy_t)::ELEM;

# define TEST_FWD_TYPEDEFS_POLICY_PARTIAL(PARENT,SEQ)                   \
  typedef typename Policies<PARENT>::TPolicy_t::T T;                    \
  BOOST_PP_SEQ_FOR_EACH(TEST_FWD_TYPEDEFS_POLICY_OPTIONAL, PARENT, SEQ) \
  struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n

namespace test_policy
{
  template <typename TRAITS>
  struct TPolicy
  {
  protected:
    typedef typename TRAITS::T T;
  };

  template <typename TRAITS>
  struct NPolicy
  {
  protected:
    static const int N = TRAITS::N;
  };

  template <typename TRAITS>
  struct MPolicy
  {
  protected:
    static const int M = TRAITS::M;
  };

  template <typename TRAITS>
  struct Policies
  {
    typedef TPolicy<TRAITS> TPolicy_t;
    typedef NPolicy<TRAITS> NPolicy_t;
    typedef MPolicy<TRAITS> MPolicy_t;
  };

  template <typename U>
  class Foo : public Policies<U>::TPolicy_t,
              public Policies<U>::NPolicy_t,
              public Policies<U>::MPolicy_t
  {
  public:
    TEST_FWD_TYPEDEFS_POLICY (U);

    Foo () : t_ (12.)
    {
      std::cout << "Foo.t  = " << t_ << std::endl;
      std::cout << "Foo.N  = " << N << std::endl;
      std::cout << "Foo.M  = " << M << std::endl;
    }

  public:
    T t_;
  };

  template <typename U>
  class Bar : public Policies<U>::TPolicy_t,
              public Policies<U>::NPolicy_t
  {
  public:
    TEST_FWD_TYPEDEFS_POLICY_PARTIAL (U,(N));

    Bar (const Foo<U>& foo)
      : t1_ (foo.t_),
        t2_ (21.)
    {
      std::cout << "Bar.t1 = " << t1_ << std::endl;
      std::cout << "Bar.t2 = " << t2_ << std::endl;
      std::cout << "Bar.N  = " << N << std::endl;
      // Here M is not accessible (protected), even with typedefs.
      //std::cout << "Bar.M  = " << M << std::endl;
    }

  public:
    T t1_;
    T t2_;
  };
} // end of namespace test_policy

// -------------------------------------------------------------------------- //
// --                          TEST MAIN                                   -- //
// -------------------------------------------------------------------------- //

int main ()
{
  typedef TestTraits<float, 5, 42> traits_t;

  // Test mixin version
  {
    using namespace test_mixin;

    std::cout << "Mixin:" << std::endl;
    Foo<traits_t> foo;
    Bar<traits_t> bar (foo);
  }

  std::cout << std::endl;

  // Test policy-based version
  {
    using namespace test_policy;

    std::cout << "Policy-based:" << std::endl;
    Foo<traits_t> foo;
    Bar<traits_t> bar (foo);
  }

  return 0;
}
