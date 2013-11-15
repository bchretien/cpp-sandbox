#include <iostream>

#include <boost/bind.hpp>

#include <boost/fusion/include/at_c.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/zip_view.hpp>
#include <boost/fusion/include/vector.hpp>

#include <Eigen/Core>

namespace demo
{
// Data structure
template<typename T, int SIZE1, int SIZE2>
struct data
{
    T ar1[SIZE1][SIZE2];
    T ar2[SIZE1][SIZE2];
};

// Eigen Map to C-style arrays
template<typename T>
struct EigenMap
{
    typedef Eigen::Map<Eigen::Matrix<T, Eigen::Dynamic, 1> > type;
};

// Eigen Maps applied to data structures
template<typename T>
struct data_eigen
{
    template <int SIZE1, int SIZE2>
    data_eigen(data<T,SIZE1,SIZE2>& src)
        : ar1(typename EigenMap<T>::type(&src.ar1[0][0], SIZE1*SIZE2)),
          ar2(typename EigenMap<T>::type(&src.ar2[0][0], SIZE1*SIZE2))
    {
    }

    typename EigenMap<T>::type ar1;
    typename EigenMap<T>::type ar2;
};


// Print operator
struct print
{
    template<typename T>
    void operator()(const Eigen::Map<Eigen::Matrix<T, Eigen::Dynamic, 1> >& t) const
    {
        Eigen::IOFormat CommaInitFmt(Eigen::StreamPrecision,
                                     Eigen::DontAlignCols,
                                     ", ", ", ", "", "", "    ", "");
        std::cout << t.transpose().format(CommaInitFmt) << std::endl;
    }
};

// Scalar multiplication operator
struct scalarMult
{
    template<typename T, typename U>
    void operator()(T& t, U& u) const
    {
        t *= u;
    }
};

// Addition operator
struct add
{
    template <typename ZipView>
    void operator() (const ZipView& t) const
    {
        boost::fusion::at_c<2>(t) = boost::fusion::at_c<0> (t)
                                  + boost::fusion::at_c<1> (t);
    }
};

} // namespace demo

// Prepare boost::fusion code (in global namespace)
BOOST_FUSION_ADAPT_TPL_STRUCT
(
    (T),
    (demo::data_eigen) (T),
    (typename demo::EigenMap<T>::type, ar1)
    (typename demo::EigenMap<T>::type, ar2)
)

int main()
{
    typedef float REALTYPE;
    const int SIZE1 = 2;
    const int SIZE2 = 2;

    // Basic data structure with multidimensional arrays
    demo::data<REALTYPE, SIZE1, SIZE2> d1;
    for (uint i = 0; i < SIZE1; ++i)
        for (uint j = 0; j < SIZE2; ++j)
        {
            d1.ar1[i][j] = (REALTYPE)((i+1)*(j+1));
            d1.ar2[i][j] = (REALTYPE)(i + j);
        }
    demo::data<REALTYPE, SIZE1, SIZE2> d2;
    demo::data<REALTYPE, SIZE1, SIZE2> d3;
    memset(&d3, 0, sizeof(demo::data<REALTYPE, SIZE1, SIZE2>));

    for (uint i = 0; i < SIZE1; ++i)
        for (uint j = 0; j < SIZE2; ++j)
        {
            d2.ar1[i][j] = 1.0;
            d2.ar2[i][j] = 2.0;
        }

    // Eigen::Map + BOOST_FUSION_ADAPT_TPL_STRUCT
    demo::data_eigen<REALTYPE> eig_d1 (d1);
    demo::data_eigen<REALTYPE> eig_d2 (d2);
    demo::data_eigen<REALTYPE> eig_d3 (d3);

    // Print test
    std::cout << "d1:" << std::endl;
    boost::fusion::for_each (eig_d1, demo::print ());
    std::cout << std::endl;

    // Scalar multiplication test
    boost::fusion::for_each (eig_d1, boost::bind<void>
                                     (demo::scalarMult (), _1, (REALTYPE)(2.0)));
    std::cout << "d1 *= 2:" << std::endl;
    boost::fusion::for_each (eig_d1, demo::print ());
    std::cout << std::endl;

    // Addition test
    typedef demo::data_eigen<REALTYPE>& vector_ref;
    typedef boost::fusion::vector<vector_ref,vector_ref,vector_ref> data_zip;

    boost::fusion::for_each (boost::fusion::zip_view<data_zip>
                             (data_zip (eig_d1, eig_d2, eig_d3)),
                             demo::add());

    std::cout << "d2:" << std::endl;
    boost::fusion::for_each (eig_d2, demo::print ());
    std::cout << std::endl;

    std::cout << "d3 = d1 + d2:" << std::endl;
    boost::fusion::for_each (eig_d3, demo::print());
    std::cout << std::endl;

    return EXIT_SUCCESS;
}
