#include <iostream>
#include <vector>
#include <assert.h>

// Boost
#include <boost/bind.hpp>

#include <boost/fusion/include/at_c.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/zip_view.hpp>
#include <boost/fusion/include/vector.hpp>

#include <boost/preprocessor/array/elem.hpp>
#include <boost/preprocessor/iteration/local.hpp>

// Eigen
#include <Eigen/Core>

#define SIZE1 2
#define SIZE2 2

namespace demo
{

// Low-level data structures
template <typename T>
struct data1_t
{
    T ar1[SIZE1][SIZE2];
    T ar2[SIZE1][SIZE2];
};

template <typename T>
struct data2_t
{
    T ar1[SIZE2][SIZE1];
    T ar2[SIZE2][SIZE1];
};

// High-level wrapper
template <typename T>
struct wrapper
{
    std::vector<data1_t<T> > data1;
    std::vector<data2_t<T> > data2;
};

// Eigen Map to C-style arrays
template <typename T>
struct EigenMap
{
    typedef Eigen::Map<Eigen::Matrix<T, Eigen::Dynamic, 1> > type;
};

// Eigen Maps applied to data structures
template <typename T>
struct data1_eigen
{
    typedef data1_t<T> base_type;

    data1_eigen (base_type& src)
        : ar1 (typename EigenMap<T>::type (&src.ar1[0][0], SIZE1*SIZE2)),
          ar2 (typename EigenMap<T>::type (&src.ar2[0][0], SIZE1*SIZE2))
    {
    }

    typename EigenMap<T>::type ar1;
    typename EigenMap<T>::type ar2;
};

template <typename T>
struct data2_eigen
{
    typedef data2_t<T> base_type;

    data2_eigen (base_type& src)
        : ar1 (typename EigenMap<T>::type (&src.ar1[0][0], SIZE1*SIZE2)),
          ar2 (typename EigenMap<T>::type (&src.ar2[0][0], SIZE1*SIZE2))
    {
    }

    typename EigenMap<T>::type ar1;
    typename EigenMap<T>::type ar2;
};


#define N_DATA                   2
#define DATA_NAMES               (N_DATA, (data1, data2))
#define DATA_TYPES               (N_DATA, (data1_eigen<T>, data2_eigen<T>))

#define GET_DATA_NAME(n)         BOOST_PP_ARRAY_ELEM(n, DATA_NAMES)
#define GET_SRC_DATA_NAME(src,n) src.GET_DATA_NAME(n)
#define GET_DATA_TYPE(n)         BOOST_PP_ARRAY_ELEM(n, DATA_TYPES)
#define WRAPPER_EIGEN_VAR(n)     std::vector<GET_DATA_TYPE(n)> GET_DATA_NAME(n)

// Wrapper of Eigen Maps applied to data structures
// Three lines only need to be updated when data vectors are added:
//   - N_DATA: the number of data vectors
//   - DATA_NAMES: the names of the variables holding these vectors
//   - DATA_TYPES: the types of the vectors
template <typename T>
struct wrapper_eigen
{
    template <typename U, typename V>
    void prepare_map (std::vector<U>& src_data, std::vector<V>& dst_data)
    {
        for (typename std::vector<typename V::base_type>::iterator
             it  = src_data.begin ();
             it != src_data.end ();
             ++it)
            dst_data.push_back (V (*it));
    }

    wrapper_eigen (wrapper<T>& src)
    {
        // PREPROCESSOR LOOP FOR THE PREPARATION OF THE EIGEN MAPS
        // What macro to call in the loop
        #define BOOST_PP_LOCAL_MACRO(n)   \
            prepare_map (GET_SRC_DATA_NAME(src,n), GET_DATA_NAME(n));
        // Loop limits (from 0 to N_DATA)
        #define BOOST_PP_LOCAL_LIMITS     (0,N_DATA-1)
        // Execute the loop
        #include BOOST_PP_LOCAL_ITERATE()
    }

    // PREPROCESSOR LOOP FOR THE DEFINITION OF THE VARIABLES
    // What macro to call in the loop
    #define BOOST_PP_LOCAL_MACRO(n)   WRAPPER_EIGEN_VAR(n);
    // Loop limits (from 0 to N_DATA)
    #define BOOST_PP_LOCAL_LIMITS     (0,N_DATA-1)
    // Execute the loop
    #include BOOST_PP_LOCAL_ITERATE()
};

// Print operator
struct print
{
    template <typename T>
    void operator() (const Eigen::Map<Eigen::Matrix<T, Eigen::Dynamic, 1> >& t) const
    {
        Eigen::IOFormat CommaInitFmt(Eigen::StreamPrecision,
                                     Eigen::DontAlignCols,
                                     ", ", ", ", "", "", "    ", "");
        std::cout << t.transpose().format(CommaInitFmt) << std::endl;
    }
};

// Scalar multiplication operator
template <typename T>
struct scalar_mult
{
    scalar_mult (T val) : val_(val) {}

    template<typename U>
    void operator() (U& u) const
    {
        u *= val_;
    }

    T val_;
};

// Addition operator
struct add
{
    template <typename ZipView>
    void operator() (const ZipView& t) const
    {
        boost::fusion::at_c<2>(t) = boost::fusion::at_c<0>(t) + boost::fusion::at_c<1>(t);
    }
};

// Apply function on wrapper
template <typename FUNC>
struct wrapper_op
{
    template <typename T>
    void operator() (std::vector<T>& v) const
    {
        for (typename std::vector<T>::iterator
             it  = v.begin ();
             it != v.end ();
             ++it)
        {
            boost::fusion::for_each (*it, FUNC ());
        }
    }

    template <typename T, typename U>
    void operator() (std::vector<T>& v, U& u) const
    {
        for (typename std::vector<T>::iterator
             it  = v.begin ();
             it != v.end ();
             ++it)
        {
            boost::fusion::for_each (*it, FUNC (u));
        }
    }

    template <typename T>
    void operator() (const boost::fusion::vector3<std::vector<T>&,
                                                  std::vector<T>&,
                                                  std::vector<T>& >& t) const
    {
        typedef boost::fusion::vector<T&,T&,T&> type_zip;

        std::vector<T>& v0 = boost::fusion::at_c<0>(t);
        std::vector<T>& v1 = boost::fusion::at_c<1>(t);
        std::vector<T>& v2 = boost::fusion::at_c<2>(t);

        assert (v0.size () == v1.size ());
        assert (v0.size () == v2.size ());

        for (size_t i = 0; i < v0.size (); ++i)
        {
            boost::fusion::for_each (boost::fusion::zip_view<type_zip>
                                     (type_zip (v0[i], v1[i], v2[i])),
                                     demo::add ());
        }
    }
};

} // namespace demo

// Prepare boost::fusion code (in global namespace)
BOOST_FUSION_ADAPT_TPL_STRUCT
(
    (T),
    (demo::data1_eigen) (T),
    (typename demo::EigenMap<T>::type, ar1)
    (typename demo::EigenMap<T>::type, ar2)
)

BOOST_FUSION_ADAPT_TPL_STRUCT
(
    (T),
    (demo::data2_eigen) (T),
    (typename demo::EigenMap<T>::type, ar1)
    (typename demo::EigenMap<T>::type, ar2)
)

BOOST_FUSION_ADAPT_TPL_STRUCT
(
    (T),
    (demo::wrapper_eigen) (T),
    (typename std::vector<demo::data1_eigen<T> >, data1)
    (typename std::vector<demo::data2_eigen<T> >, data2)
)

int main()
{
    typedef float REALTYPE;

    // Basic data structure with vectors of multidimensional arrays
    demo::wrapper<REALTYPE> w1;
    demo::data1_t<REALTYPE> data1;
    demo::data2_t<REALTYPE> data2;
    for (uint i = 0; i < SIZE1; ++i)
        for (uint j = 0; j < SIZE2; ++j)
        {
            data1.ar1[i][j] = (REALTYPE)(i*SIZE1+j);
            data1.ar2[i][j] = (REALTYPE)(2.0*(i*SIZE1+j));
            data2.ar1[i][j] = (REALTYPE)(0.1*data1.ar1[i][j]);
            data2.ar2[i][j] = (REALTYPE)(0.1*data1.ar2[i][j]);
        }
    w1.data1.push_back (data1);
    w1.data2.push_back (data2);

    demo::wrapper<REALTYPE> w2;

    for (uint i = 0; i < SIZE1; ++i)
        for (uint j = 0; j < SIZE2; ++j)
        {
            data1.ar1[i][j] = 1.0;
            data1.ar2[i][j] = 2.0;
            data2.ar1[i][j] = 1.0;
            data2.ar2[i][j] = 2.0;
        }
    w2.data1.push_back (data1);
    w2.data2.push_back (data2);

    demo::wrapper<REALTYPE> w3;
    w3.data1.resize (1);
    w3.data2.resize (1);
    memset (&w3.data1[0], 0, sizeof (demo::data1_t<REALTYPE>));
    memset (&w3.data2[0], 0, sizeof (demo::data2_t<REALTYPE>));

    // Eigen::Map + BOOST_FUSION_ADAPT_TPL_STRUCT
    demo::wrapper_eigen<REALTYPE> eig_w1 (w1);
    demo::wrapper_eigen<REALTYPE> eig_w2 (w2);
    demo::wrapper_eigen<REALTYPE> eig_w3 (w3);

    // Print test
    std::cout << "w1:" << std::endl;
    boost::fusion::for_each (eig_w1, demo::wrapper_op<demo::print> ());
    std::cout << std::endl;

    // Scalar multiplication test
    boost::fusion::for_each (eig_w1, boost::bind<void> (demo::wrapper_op<demo::scalar_mult<REALTYPE> > (), _1, 2.0));
    std::cout << "w1 *= 2:" << std::endl;
    boost::fusion::for_each (eig_w1, demo::wrapper_op<demo::print> ());
    std::cout << std::endl;

    // Addition test
    typedef demo::wrapper_eigen<REALTYPE>& vector_ref;
    typedef boost::fusion::vector<vector_ref,vector_ref,vector_ref> wrapper_zip;

    boost::fusion::for_each (boost::fusion::zip_view<wrapper_zip>
                             (wrapper_zip (eig_w1, eig_w2, eig_w3)),
                             demo::wrapper_op<demo::add> ());

    std::cout << "w2:" << std::endl;
    boost::fusion::for_each (eig_w2, demo::wrapper_op<demo::print> ());
    std::cout << std::endl;

    std::cout << "w3 = w1 + w2:" << std::endl;
    boost::fusion::for_each (eig_w3, demo::wrapper_op<demo::print> ());
    std::cout << std::endl;

    return EXIT_SUCCESS;
}
