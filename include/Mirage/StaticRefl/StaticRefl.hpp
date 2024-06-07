#pragma once

#include "Mirage/Util/FunctionTraits.hpp"
#include "Mirage/Util/TypeList.hpp"
#include "Mirage/Util/VariableTraits.hpp"

namespace mirage::srefl
{
    /**
     * @brief attributes that attach to field/function
     */
    template <typename... Attrs>
    using AttrList = util::TypeList<Attrs...>;

    namespace internal
    {
        template <typename T, bool>
        struct BasicFieldTraits;

        template <typename T>
        struct BasicFieldTraits<T, true> : util::FunctionTraits<T>
        { };

        template <typename T>
        struct BasicFieldTraits<T, false> : util::VariableTraits<T>
        { };
    }    // namespace internal

    /**
     * @brief extract class field (member variable, member function) info.
     * @tparam T
     * @tparam AttrList
     * @tparam Next
     * @tparam Type
     */
    template <auto T,
              typename AttrList = AttrList<>,
              typename Next     = void,
              typename Type     = decltype(T)>
    struct FieldTraits : internal::BasicFieldTraits<Type, util::is_function_v<Type>>
    {
        using next                    = Next;
        static constexpr auto pointer = T;
    };

    /**
     * @brief store class constructor
     * @tparam Args
     */
    template <typename... Args>
    struct Ctor
    {
        using args = util::TypeList<Args...>;
    };

    /**
     * @brief store base classes
     * @tparam Bases
     */
    template <typename... Bases>
    struct Base
    {
        using bases = util::TypeList<Bases...>;
    };

    template <typename T>
    struct BaseTypeInfo
    {
        using type                    = T;
        static constexpr bool isFinal = std::is_final_v<T>;
    };

    /**
     * @brief store class type info
     * @tparam T
     */
    template <typename T>
    struct TypeInfo;
}    // namespace mirage::srefl
