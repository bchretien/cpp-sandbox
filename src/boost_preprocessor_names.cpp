#include <iostream>

// Boost
#include <boost/mpl/vector.hpp>
#include <boost/mpl/at.hpp>

#include <boost/preprocessor/array/elem.hpp>
#include <boost/preprocessor/iteration/local.hpp>

namespace demo
{

struct data
{
    typedef boost::mpl::vector<int,int> data_types;

    boost::mpl::at_c<data_types,0>::type a;
    boost::mpl::at_c<data_types,1>::type b;
};

}

template <typename T>
void print_res (const std::string& str1, T val1,
                const std::string& str2, T val2)
{
    std::cout << str1 << " = " << val1 << ", ";
    std::cout << str2 << " = " << val2 << std::endl;
}

int main()
{
    demo::data d = {0, 0};

    print_res ("a", d.a, "b", d.b);

    // Increment "manually"
    d.a += 1;
    d.b += 1;

    print_res ("a", d.a, "b", d.b);

    // Increment thanks to macro and concatenation
    #define INCREMENT(s,VAR) s.VAR += 1
    INCREMENT (d, a);
    INCREMENT (d, b);

    print_res ("a", d.a, "b", d.b);

    // Increment thanks to Boost preprocessor
    #define DATA_NAMES (2, (a, b))
    INCREMENT (d, BOOST_PP_ARRAY_ELEM(0, DATA_NAMES));
    INCREMENT (d, BOOST_PP_ARRAY_ELEM(1, DATA_NAMES));

    print_res ("a", d.a, "b", d.b);

    // Increment and loop thanks to Boost preprocessor
    #define BOOST_PP_LOCAL_MACRO(n)   \
        INCREMENT (d, BOOST_PP_ARRAY_ELEM(n, DATA_NAMES));
    // Loop limits (from 0 to N_DATA)
    #define BOOST_PP_LOCAL_LIMITS     (0,1)
    // Execute the loop
    #include BOOST_PP_LOCAL_ITERATE()

    print_res ("a", d.a, "b", d.b);

    return EXIT_SUCCESS;
}
