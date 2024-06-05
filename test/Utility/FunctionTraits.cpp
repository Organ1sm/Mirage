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
    static_assert(std::is_same_v<function_type_t<globFunc>, void(float, std::string)>);
}
