#include <Mirage/Util/FunctionTraits.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace mirage::util;
void globFunc(float, std::string);

struct Clazz
{
    std::string Foo();
    void constFoo(double) const;
    static int &staticFoo(float);
};

TEST_CASE("function pointer type")
{
    static_assert(
        std::is_same_v<function_type_from_pointer_t<globFunc>, void(float, std::string)>);
    static_assert(
        std::is_same_v<function_pointer_type_t<&Clazz::Foo>, std::string (Clazz::*)()>);
    static_assert(std::is_same_v<function_pointer_type_t<Clazz::staticFoo>, int &(*)(float)>);
    static_assert(std::is_same_v<function_pointer_type_t<&Clazz::constFoo>,
                                 void (Clazz::*)(double) const>);
}

TEST_CASE("function type from pointer")
{
    static_assert(
        std::is_same_v<function_type_from_pointer_t<globFunc>, void(float, std::string)>);
    static_assert(
        std::is_same_v<function_type_from_pointer_t<&Clazz::Foo>, std::string (Clazz::*)()>);
    static_assert(
        std::is_same_v<function_type_from_pointer_t<Clazz::staticFoo>, int &(float)>);
    static_assert(std::is_same_v<function_type_from_pointer_t<&Clazz::constFoo>,
                                 void (Clazz::*)(double) const>);
}

TEST_CASE("function pointer traits")
{
    SECTION("global function")
    {
        using traits = function_pointer_traits<globFunc>;
        static_assert(std::is_same_v<traits::args, TypeList<float, std::string>>);
        static_assert(!traits::is_const);
        static_assert(!traits::is_member);
        static_assert(std::is_same_v<traits::pointer, void (*)(float, std::string)>);
        static_assert(std::is_same_v<traits::return_type, void>);
        static_assert(std::is_same_v<traits::type, void(float, std::string)>);
    }

    SECTION("member function")
    {
        using traits = function_pointer_traits<&Clazz::Foo>;
        static_assert(std::is_same_v<traits::args, TypeList<>>);
        static_assert(!traits::is_const);
        static_assert(traits::is_member);
        static_assert(std::is_same_v<traits::pointer, std::string (Clazz::*)(void)>);
        static_assert(std::is_same_v<traits::return_type, std::string>);
        static_assert(std::is_same_v<traits::type, std::string (Clazz::*)(void)>);
    }

    SECTION("const member function")
    {
        using traits = function_pointer_traits<&Clazz::constFoo>;
        static_assert(std::is_same_v<traits::args, TypeList<double>>);
        static_assert(traits::is_const);
        static_assert(traits::is_member);
        static_assert(std::is_same_v<traits::pointer, void (Clazz::*)(double) const>);
        static_assert(std::is_same_v<traits::return_type, void>);
    }

    SECTION("static function")
    {
        using traits = function_pointer_traits<Clazz::staticFoo>;
        static_assert(std::is_same_v<traits::args, TypeList<float>>);
        static_assert(!traits::is_const);
        static_assert(!traits::is_member);
        static_assert(std::is_same_v<traits::pointer, int &(*)(float)>);
        static_assert(std::is_same_v<traits::return_type, int &>);
        static_assert(std::is_same_v<traits::type, int &(float)>);
    }
}
