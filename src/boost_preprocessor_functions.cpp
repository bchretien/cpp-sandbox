#include <boost/preprocessor/array/elem.hpp>
#include <boost/preprocessor/iteration/local.hpp>
#include <boost/preprocessor/cat.hpp>

#include <iostream>

#define N_DATA        4
#define DATA_NAMES    (N_DATA, (P0, P1, P2, P3))
#define DATA_SIZE     (N_DATA, (m, 3*m, m*m, 1+m))
#define GET_NAME(n)   BOOST_PP_ARRAY_ELEM(n, DATA_NAMES)
#define GET_SIZE(n)   BOOST_PP_ARRAY_ELEM(n, DATA_SIZE)
#define SIZE_FUNC(n)  inline size_t BOOST_PP_CAT(GET_NAME(n), _size) \
                      (size_t m) { \
                          return GET_SIZE(n); \
                      }
#define PRINT_SIZE(n) std::cout << detail::BOOST_PP_CAT(GET_NAME(n), _size) (m) \
                                << std::endl;

namespace detail {
    // Generate functions thanks to Boost preprocessor
    #define BOOST_PP_LOCAL_MACRO(n) SIZE_FUNC(n)
    // Loop limits (from 0 to N_DATA-1)
    #define BOOST_PP_LOCAL_LIMITS (0,N_DATA-1)
    // Execute the loop
    #include BOOST_PP_LOCAL_ITERATE()
} // namespace detail

int main()
{
    const int m = 4;

    // Loop over functions thanks to Boost preprocessor
    #define BOOST_PP_LOCAL_MACRO(n) PRINT_SIZE(n)
    // Loop limits (from 0 to N_DATA-1)
    #define BOOST_PP_LOCAL_LIMITS (0,N_DATA-1)
    // Execute the loop
    #include BOOST_PP_LOCAL_ITERATE()

    return 0;
}
