#pragma once

#include "Mirage/StaticRefl/StaticRefl.hpp"

#define SReflClass(type) \
    template <>          \
    struct ::mirage::srefl::TypeInfo<type> : ::mirage::srefl::BaseTypeInfo<type>

#define Fields using fields = ::mirage::util::TypeList

#define Field ::mirage::srefl::FieldTraits

#define Bases using bases = typename ::mirage::util::TypeList

#define Ctors using ctors = ::mirage::util::TypeList

#define Ctor ::mirage::srefl::Ctor
