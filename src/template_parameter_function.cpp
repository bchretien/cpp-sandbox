#include <iostream>
#include <vector>
#include <string>

#include <Eigen/Core>

#include <boost/type_traits.hpp>
#include <boost/variant.hpp>
#include <boost/variant/static_visitor.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/function.hpp>
#include <boost/function_types/result_type.hpp>

typedef Eigen::DenseIndex size_type;
typedef Eigen::MatrixXd matrix_t;

class Foo
{
public:
  Foo () : m (3, 4) {}

  virtual size_type size () const = 0;
  virtual std::string name () const = 0;

protected:
  matrix_t m;
};


class Foo1 : public Foo
{
public:

  Foo1 () : Foo ()
  {}

  size_type size () const
  {
    return m.rows ();
  }

  std::string name () const
  {
    return "Foo1";
  }
};


class Foo2 : public Foo
{
public:
  Foo2 () : Foo ()
  {}

  size_type size () const
  {
    return m.cols ();
  }

  std::string name () const
  {
    return "Foo2";
  }
};


template <typename F>
struct return_type
{
  typedef typename boost::function_types::result_type<F>::type type;
};

template <typename F>
class Visitor
: public boost::static_visitor<typename return_type<F>::type>
{
public:

  typedef typename return_type<F>::type return_type;

  Visitor (F f) : f_ (f) {}

  template <typename U>
  return_type operator() (const U& u) const
  {
    return (u.*f_) ();
  }

  F f_;
};


int main()
{
  typedef boost::variant<Foo1, Foo2> foo_t;
  std::vector<foo_t> v;
  v.push_back (Foo1 ());
  v.push_back (Foo2 ());

  typedef BOOST_TYPEOF(&Foo::size) fooSize_t;
  typedef BOOST_TYPEOF(&Foo::name) fooName_t;

  for (std::vector<foo_t>::const_iterator
       iter  = v.begin ();
       iter != v.end ();
       ++iter)
  {
    Visitor<fooSize_t> sizeVisitor (&Foo::size);
    std::cout << boost::apply_visitor (sizeVisitor, *iter) << std::endl;

    Visitor<fooName_t> nameVisitor (&Foo::name);
    std::cout << boost::apply_visitor (nameVisitor, *iter) << std::endl;
  }

  return 0;
}
