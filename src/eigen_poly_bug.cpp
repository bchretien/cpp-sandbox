#include <iostream>
#include <vector>

#include <Eigen/Core>
#include <unsupported/Eigen/Polynomials>

using namespace Eigen;
using namespace std;

typedef vector<double> roots_t;

template <int N>
void test_solver_fixed (const Matrix<double,N+1,1>& coeffs)
{
  PolynomialSolver<double, N> solver (coeffs);

  roots_t roots;
  solver.realRoots (roots);

  cout << "Roots of degree " << N << " polynomial (fixed):" << endl;
  for (roots_t::const_iterator
       iter  = roots.begin ();
       iter != roots.end ();
       ++iter)
    cout << *iter << endl;
}

//template <>
//void test_solver_fixed<1> (const Matrix<double,2,1>& /*coeffs*/)
//{
//  cout << "PolynomialSolver<T,1> cannot be compiled." << endl;
//}

template <int N>
void test_solver_dyn (const Matrix<double,N+1,1>& coeffs)
{
  PolynomialSolver<double, Dynamic> solver (coeffs);

  roots_t roots;
  solver.realRoots (roots);

  cout << "Roots of degree " << N << " polynomial (dynamic):" << endl;
  for (roots_t::const_iterator
       iter  = roots.begin ();
       iter != roots.end ();
       ++iter)
    cout << *iter << endl;
}

template <int N>
void test_solver ()
{
  typedef Matrix<double,N+1,1> coeffs_t;

  coeffs_t coeffs;
  coeffs.setZero ();
  coeffs[N] = 2.;

  test_solver_fixed<N> (coeffs);
  test_solver_dyn<N>   (coeffs);
}


int main ()
{
  test_solver<4> ();
  test_solver<3> ();
  test_solver<2> ();
  test_solver<1> ();

  return 0;
}
