#pragma once

#include "Mirage/Util/TypeList.hpp"

namespace mirage::util
{
    namespace detail
    {
        template <typename T>
        struct FunctionType;

        template <typename Ret, typename... Args>
        struct FunctionType<Ret (*)(Args...)>
        {
            using type = Ret(Args...);
        };

        template <typename Ret, typename Class, typename... Args>
        struct FunctionType<Ret (Class::*)(Args...)>
        {
            using type = Ret (Class::*)(Args...);
        };

        template <typename Ret, typename Class, typename... Args>
        struct FunctionType<Ret (Class::*)(Args...) const>
        {
            using type = Ret (Class::*)(Args...) const;
        };

    }    // namespace detail

    /**
     * @brief get function type from function pointer type
     * @tparam  F
     */
    template <typename F>
    using function_type_t = typename detail::FunctionType<F>::type;
}    // namespace mirage::util
