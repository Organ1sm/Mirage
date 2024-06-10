#include "Mirage/StaticRefl/StaticRefl.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string>
#include <utility>
#include <vector>

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

#include "Mirage/StaticRefl/StaticReflBegin.hpp"
// clang-format off
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
    Fields<Field<&Person::addChild>,
           Field<&Person::children>,
           Field<&Person::getHeight>,
           Field<&Person::getName>,
           Field<&Person::name>,
           Field<&Person::operator+ >>;
};
#include "Mirage/StaticRefl/StaticReflEnd.hpp"
// clang-format on

TEST_CASE("field traits")
{
    SECTION("member function")
    {
        using traits = FieldTraits<&Person::addChild>;
        static_assert(std::is_same_v<traits::return_type, void>);
        static_assert(std::is_same_v<traits::args, TypeList<const Person &>>);
        static_assert(!traits::is_const);
        static_assert(traits::is_member);
        static_assert(traits::pointer == &Person::addChild);
    }

    SECTION("member operator function")
    {
        using traits = FieldTraits<&Person::operator+ >;
        static_assert(traits::is_member);
        static_assert(std::is_same_v<traits::return_type, Person &>);
        static_assert(std::is_same_v<traits::args, TypeList<const Person &>>);
        static_assert(traits::pointer == &Person::operator+);
    }

    SECTION("member const function")
    {
        using traits = FieldTraits<&Person::getHeight>;
        static_assert(traits::is_member);
        static_assert(traits::is_const);
        static_assert(std::is_same_v<traits::return_type, float>);
        static_assert(std::is_same_v<traits::args, TypeList<>>);
        static_assert(traits::pointer == &Person::getHeight);
    }

    SECTION("member variable")
    {
        using traits = FieldTraits<&Person::name>;
        static_assert(traits::is_member);
        static_assert(std::is_same_v<traits::type, std::string>);
        static_assert(traits::pointer == &Person::name);
    }

    SECTION("member variable")
    {
        using traits = FieldTraits<&Person::children>;
        static_assert(traits::is_member);
        static_assert(std::is_same_v<traits::type, std::vector<Person>>);
        static_assert(traits::pointer == &Person::children);
    }
    SECTION("static member variable")
    {
        using traits = FieldTraits<&Person::familyName>;
        static_assert(!traits::is_member);
        static_assert(std::is_same_v<traits::type, std::string>);
        static_assert(traits::pointer == &Person::familyName);
    }
    SECTION("macro generation")
    {
        using type_info = TypeInfo<Person>;
        static_assert(std::is_same_v<type_info::type, Person>);
        static_assert(std::is_same_v<type_info::bases, TypeList<>>);
        static_assert(
            std::is_same_v<type_info::ctors, TypeList<Ctor<const std::string &, float>>>);
        static_assert(type_info::isFinal);
        static_assert(std::is_same_v<type_info::fields,
                                     TypeList<FieldTraits<&Person::addChild>,
                                              FieldTraits<&Person::children>,
                                              FieldTraits<&Person::getHeight>,
                                              FieldTraits<&Person::getName>,
                                              FieldTraits<&Person::name>,
                                              FieldTraits<&Person::operator+ >>>);
    }
}
