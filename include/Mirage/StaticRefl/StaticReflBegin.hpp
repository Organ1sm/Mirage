/**
 * @file StaticReflBegin.hpp
 * @brief some helpful macros to help reflect class more eazier
 * @note use `StaticReflEnd.hpp` when you finish reflect.
 */

#pragma once

#define SReflClass(type) \
    template <>          \
    struct TypeInfo<type> : BaseTypeInfo<type>

#define Fields(...) inline static constexpr auto fields = std::make_tuple(__VA_ARGS__);

#define Field(pointer, ...) \
    FieldTraits { pointer, #pointer, ##__VA_ARGS__ }

#define Bases(...) using bases = util::TypeList<__VA_ARGS__>;
#define Ctors(...) using ctors = util::TypeList<__VA_ARGS__>;
#define Ctor(...)  Ctor<__VA_ARGS__>

#ifdef MIRAGE_SREFL_BEGIN
    #error \
        "do you forget include Mirage/StaticRefl/StaticReflEnd.hpp after include Mirage/StaticRefl/StaticReflBegin.hpp?"
    #define MIRAGE_SREFL_BEGIN
#endif
