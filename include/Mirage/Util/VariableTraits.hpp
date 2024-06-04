#pragma once

#include "VariableTraits.hpp"

#include <type_traits>

namespace mirage::util
{
    namespace detail
    {
        template <typename T>
        struct VariableType
        {
            using type = T;
        };

        template <typename Class, typename T>
        struct VariableType<T Class::*>
        {
            using type = T;
        };
    }    // namespace detail

    template <typename T>
    using variable_type_t = typename detail::VariableType<T>::type;

    namespace detail
    {
        template <typename T>
        auto variable_pointer_to_type(long, T *) -> T;

        template <typename Class, typename T>
        auto variable_pointer_to_type(char, T Class::*) -> T;
    }    // namespace detail

    /**
     * @brief get a varibale type from it's variale pointer
     */
    template <auto V>
    using variable_pointer_to_type_t = decltype(detail::variable_pointer_to_type(0, V));

    namespace internal
    {
        template <typename T>
        struct BasicVariableTraits
        {
            using type                      = variable_type_t<T>;
            static constexpr bool is_member = std::is_member_pointer_v<T>;
        };
    }    // namespace internal

    /**
     * @brief extract variable info from variable type
     * @tparam T Func
     */
    template <typename T>
    struct VariableTraits;

    template <typename T>
    struct VariableTraits<T *> : internal::BasicVariableTraits<T>
    {
        using pointer = T *;
    };

    template <typename Class, typename T>
    struct VariableTraits<T Class::*> : internal::BasicVariableTraits<T Class::*>
    {
        using pointer = T Class::*;
    };

    namespace detail
    {
        template <auto V>
        struct VariablePointerTraits : VariableTraits<decltype(V)>
        { };
    }    // namespace detail

    template <auto V>
    using variable_pointer_traits = detail::VariablePointerTraits<V>;
}    // namespace mirage::util
