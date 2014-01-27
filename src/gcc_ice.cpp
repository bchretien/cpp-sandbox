// Compilation (gcc 4.8.2):
//   g++ gcc_ice.cpp
//
// Internal compiler error:
//   gcc_ice.cpp: In instantiation of ‘class Foo<float, 5>’:
//   gcc_ice.cpp:35:28:   required from here
//   gcc_ice.cpp:28:17: internal compiler error: unexpected expression ‘P_S’ of kind template_parm_index
//     friend void Bar<U, P_S, A_S>::impl (const Foo<U,P_S>& poly);
//                 ^

template <typename T, int S> class Foo;

template <typename T, int P_S, int A_S>
struct Bar
{
    static void impl (const Foo<T,P_S>& poly)
    {
    }
};

template <typename T, int S>
class Foo {

    Foo ();
    ~Foo ();

    template <typename U, int P_S, int A_S>
    friend void Bar<U, P_S, A_S>::impl (const Foo<U,P_S>& poly);
};

int main()
{
    Foo<float,5> f ();

    Bar<float,5,2>::impl (f);

    return 0;
}
