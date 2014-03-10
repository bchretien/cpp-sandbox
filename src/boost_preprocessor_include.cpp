#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/stringize.hpp>

#define INCLUDE_FILE(M) BOOST_PP_STRINGIZE(BOOST_PP_CAT(M, _impl_win.hpp))

// expands to: #include "module_impl_win.hpp"
#include INCLUDE_FILE(module)

int main()
{
    test<int> ();

    return 0;
}
