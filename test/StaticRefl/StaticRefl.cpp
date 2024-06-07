#include "Mirage/StaticRefl/StaticRefl.hpp"

#include <Mirage/StaticRefl/StaticReflBegin.hpp>
#include <string>
#include <utility>
#include <vector>

using namespace mirage::util;

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
{
    Bases<>;
    Ctors<Ctor<const std::string &, float>>;
    Fields<Field<&Person::addChild>,
           Field<&Person::children>,
           Field<&Person::getHeight>,
           Field<&Person::getName>,
           Field<&Person::name>,
           Field<&Person::operator+ >>;
};
#include "Mirage/StaticRefl/StaticReflEnd.hpp"
// clang-format on
