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
        {
            constexpr bool isConstMember() const noexcept
            {
                return util::FunctionTraits<T>::is_const;
            }

            constexpr bool isMember() const noexcept
            {
                return util::FunctionTraits<T>::is_member;
            }

            constexpr bool isFunction() const noexcept { return true; }

            constexpr bool isVariable() const noexcept { return false; }
        };

        template <typename T>
        struct BasicFieldTraits<T, false> : util::VariableTraits<T>
        {
            constexpr bool isConstMember() const noexcept { return false; }

            constexpr bool isMember() const noexcept
            {
                return util::VariableTraits<T>::is_member;
            }

            constexpr bool isFunction() const noexcept { return false; }

            constexpr bool isVariable() const noexcept { return true; }
        };
    }    // namespace internal

    /**
     * @brief strip class/function/variable name from namespace/class prefix to pure name
     * @param name
     * @return striped name
     */
    constexpr std::string_view stripName(std::string_view name)
    {
        if (const auto idx = name.find_last_of('&'); idx != std::string_view::npos)
            name = name.substr(idx + 1, name.length());
        if (const auto idx = name.find_last_of(':'); idx != std::string_view::npos)
            name = name.substr(idx + 1, name.length());
        if (const auto idx = name.find_first_of(')'); idx != std::string_view::npos)
            name = name.substr(0, idx);
        return name;
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
          : pointer_(std::forward<T>(pointer)), name_(stripName(name)),
            attrs_(std::forward<Attrs>(attrs)...)
        { }

        /**
         * @brief check whether field is a const member (class const function)
         */
        constexpr bool isConstMember() const noexcept { return base::isConstMember(); }

        /**
         * @brief  check whether field is class member or static/global
         */
        constexpr bool isMember() const noexcept { return base::isMember(); }

        constexpr auto getName() const noexcept { return name_; }

        constexpr auto getPointer() const noexcept { return pointer_; }

        constexpr auto &getAttrs() const noexcept { return attrs_; }

      private:
        using base = internal::BasicFieldTraits<T, util::is_function_v<T>>;

        T pointer_;
        std::string_view name_;
        std::tuple<Attrs...> attrs_;
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
