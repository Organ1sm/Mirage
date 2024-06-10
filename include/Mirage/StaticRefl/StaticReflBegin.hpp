/**
 * @file StaticReflBegin.hpp
 * @brief some helpful macros to help reflect class more eazier
 * @note use `StaticReflEnd.hpp` when you finish reflect.
 */

#pragma once
#include "Mirage/StaticRefl/StaticRefl.hpp"

#define SReflClass(type, ...) \
    template <>               \
    struct TypeInfo<type, AttrList<##__VA_ARGS__>> : BaseTypeInfo<type>

#define Fields(...) inline static constexpr auto fields = std::make_tuple(__VA_ARGS__);

#define Field(pointer, ...) \
    FieldTraits { pointer, #pointer, ##__VA_ARGS__ }

#define Bases using bases = util::TypeList
#define Ctors using ctors = util::TypeList

#ifdef MIRAGE_SREFL_BEGIN
    #error \
        "do you forget include Mirage/StaticRefl/StaticReflEnd.hpp after include Mirage/StaticRefl/StaticReflBegin.hpp?"
    #define MIRAGE_SREFL_BEGIN
#endif

namespace mirage::srefl
{
