#include <iostream>

#include <Eigen/Core>
#include <Eigen/LU>
#include <Eigen/QR>

#include <cuda.h>

using namespace Eigen;
#define ROWCOL_MAJOR RowMajor

__global__ void dummy_kernel (int* data, int n)
{
    data[threadIdx.x] = threadIdx.x;
}


template <typename T>
Matrix<T,3,3> skewSymmetric (Matrix<T,3,1> v)
{
    Matrix<T,3,3> m;
    m.setZero ();

    m (0,1) = -v (2);
    m (1,0) = -m (1,0);

    m (0,2) =  v (1);
    m (2,0) = -m (0,2);

    m (1,2) = -v (0);
    m (2,1) = -m (1,2);

    return m;
}

int main ()
{
    using namespace std;

    typedef float T;
    typedef Matrix<T,Dynamic,Dynamic,ROWCOL_MAJOR> matrix_t;
    typedef Matrix<T,3,1> vector3_t;
    typedef Matrix<T,6,6,ROWCOL_MAJOR> matrix6_t;

    Matrix<T,12,6> A;

    vector3_t v1;
    v1 << (T)0., (T)0., (T)-0.106;

    Matrix<T,4,3,ROWCOL_MAJOR> B;
    B << (T)0.1, (T)0.06, (T)0.,
      (T)-0.08, (T)0.06, (T)0.,
      (T)-0.08, (T)-0.04, (T)0.,
      (T)0.1, (T)-0.04, (T)0.;

    for (size_t i = 0; i < 4; ++i)
    {
        A.block (3*i,0,3,3).setIdentity ();
        A.block (3*i,3,3,3) = skewSymmetric<T> (v1 + B.row (i).transpose ());
    }

    cout << "A =\n" << A << endl;

    cout << "## FullPivHouseholderQR(A^T) ##" << endl;
    FullPivHouseholderQR<matrix_t> qr (A.transpose ());
    cout << "Q =\n" << qr.matrixQ () << endl;
    matrix_t R = qr.matrixQR ().triangularView<Upper> ().toDenseMatrix ();
    cout << "R =\n" << R << endl;
    cout << "P =\n" << qr.colsPermutation ().toDenseMatrix () << endl;
    cout << "A.T P - Q R\n" << A.transpose () * qr.colsPermutation ()
                               - qr.matrixQ () * R
                            << endl;
    matrix6_t R1 = R.block (0, 0, 6, 6);
    cout << "R1 =\n" << R1 << std::endl;
    cout << "inv(R1) =\n" << R1.inverse () << std::endl;

    return 0;
}
