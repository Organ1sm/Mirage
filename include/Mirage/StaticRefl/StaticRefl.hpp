#pragma once

#include "Mirage/Util/FunctionTraits.hpp"
#include "Mirage/Util/TypeList.hpp"
#include "Mirage/Util/VariableTraits.hpp"

#include <string_view>

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

    constexpr std::string_view stripName(const std::string_view name)
    {
        auto idx = name.find_last_of(":");
        if (idx == std::string_view::npos)
        {
            idx = name.find_last_of('&');
            if (idx == std::string_view::npos)
                return name;
            return name.substr(idx + 1, name.length());
        }
        return name.substr(idx + 1, name.length());
    }

    /**
     * @brief extract class field (member variable, member function) info.
     * @tparam T
     * @tparams Attrs
     */
    template <typename T, typename... Attrs>
    struct FieldTraits : internal::BasicFieldTraits<T, util::is_function_v<T>>
    {
        explicit constexpr FieldTraits(T &&pointer, std::string_view name, Attrs &&...attrs)
          : pointer(std::forward<T>(pointer)), name(stripName(name)),
            attrs(std::forward<Attrs>(attrs)...)
        { }

        T pointer;
        std::string_view name;
        std::tuple<Attrs...> attrs;
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
    template <typename T, typename... Attrs>
    struct TypeInfo;
}    // namespace mirage::srefl
