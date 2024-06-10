#include "Mirage/StaticRefl/StaticRefl.hpp"

#include "Mirage/DynamicRefl/Any.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string>
#include <utility>
#include <vector>

namespace util = mirage::util;
using namespace mirage::util;
using namespace mirage::srefl;

class Person final
{
  public:
    static std::string familyName;

    Person(std::string name, const float height) : name(std::move(name)), height(height) { }

    void addChild(const Person &p) { children.push_back(p); }

    std::string getName() const { return name; }

    float getHeight() const { return height; }

    Person &operator+ (const Person &child)
    {
        addChild(child);
        return *this;
    }

    std::string name;
    float height;
    std::vector<Person> children;
};

std::string Person::familyName = "litter home";

// clang-format off
#include "Mirage/StaticRefl/StaticReflBegin.hpp"
SReflClass(Person)
// template <>
// struct srefl::TypeInfo<Person> : srefl::BaseTypeInfo<Person>
{
    // using bases = typename util::TypeList<>;
    Bases<>;
    // using ctors = util::TypeList<srefl::Ctor<const std::stirng &, float>>;
    Ctors<Ctor<const std::string &, float>>;
    // using fields = util::TypeList<Srefl::FieldTraits<&Person::addChild>,
    //                               Srefl::FieldTraits<&Person::children>,
    //                               ...
    //                               Srefl::FieldTraits<&Person::operator+ >>;

    Fields(
       Field(&Person::addChild),
       Field(&Person::children),
       Field(&Person::getHeight),
       Field(&Person::getName),
       Field(&Person::name),
       Field(&Person::operator+ )
    );
};
#include "Mirage/StaticRefl/StaticReflEnd.hpp"
// clang-format on

TEST_CASE("strip name")
{
    static_assert(stripName("Person") == "Person");
    static_assert(stripName("::Person") == "Person");
    static_assert(stripName("namespace::Person") == "Person");
    static_assert(stripName("&Person::Foo") == "Foo");
    static_assert(stripName("&Foo") == "Foo");
}

TEST_CASE("field traits")
{
    SECTION("member function")
    {
        constexpr auto traits = FieldTraits {&Person::addChild, "&Person::addChild"};
        static_assert(std::is_same_v<decltype(traits)::return_type, void>);
        static_assert(std::is_same_v<decltype(traits)::args, TypeList<const Person &>>);
        static_assert(!decltype(traits)::is_const);
        static_assert(decltype(traits)::is_member);
        static_assert(traits.pointer == &Person::addChild);
        static_assert(traits.name == "addChild");
    }

    SECTION("member operator function")
    {
        constexpr auto traits = FieldTraits {&Person::operator+, "&operator+"};
        static_assert(decltype(traits)::is_member);
        static_assert(std::is_same_v<decltype(traits)::return_type, Person &>);
        static_assert(std::is_same_v<decltype(traits)::args, TypeList<const Person &>>);
        static_assert(traits.pointer == &Person::operator+);
        static_assert(traits.name == "operator+");
    }

    SECTION("member const function")
    {
        constexpr auto traits = FieldTraits {&Person::getHeight, "&Person::getHeight"};
        static_assert(decltype(traits)::is_member);
        static_assert(decltype(traits)::is_const);
        static_assert(std::is_same_v<decltype(traits)::return_type, float>);
        static_assert(std::is_same_v<decltype(traits)::args, TypeList<>>);
        static_assert(traits.pointer == &Person::getHeight);
        static_assert(traits.name == "getHeight");
    }

    SECTION("member variable")
    {
        constexpr auto traits = FieldTraits {&Person::name, "&Person::name"};
        static_assert(decltype(traits)::is_member);
        static_assert(std::is_same_v<decltype(traits)::type, std::string>);
        static_assert(traits.pointer == &Person::name);
        static_assert(traits.name == "name");
    }

    SECTION("member variable")
    {
        constexpr auto traits = FieldTraits {&Person::children, "&Person::children"};
        static_assert(decltype(traits)::is_member);
        static_assert(std::is_same_v<decltype(traits)::type, std::vector<Person>>);
        static_assert(traits.pointer == &Person::children);
        static_assert(traits.name == "children");
    }

    SECTION("static member variable")
    {
        constexpr auto traits = FieldTraits {&Person::familyName, "&Person::familyName"};
        static_assert(!decltype(traits)::is_member);
        static_assert(std::is_same_v<decltype(traits)::type, std::string>);
        static_assert(traits.pointer == &Person::familyName);
        static_assert(traits.name == "familyName");
    }
}
