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

    namespace detail
    {
        template <typename Ret, typename... Args>
        auto FunctionPointerToType(int, Ret (*)(Args...)) -> Ret (*)(Args...);

        template <typename Ret, typename Class, typename... Args>
        auto FunctionPointerToType(char, Ret (Class::*)(Args...)) -> Ret (Class::*)(Args...);

        template <typename Ret, typename Class, typename... Args>
        auto FunctionPointerToType(char, Ret (Class::*)(Args...) const)
            -> Ret (Class::*)(Args...) const;
    }    // namespace detail

    /**
     * @brief get a function pointer type from it's function pointer
     *
     * @tparam F
     */
    template <auto F>
    using function_pointer_type_t = decltype(detail::FunctionPointerToType(0, F));

    /**
     * @brief get a function from it's function pointer
     */
    template <auto F>
    using function_type_from_pointer_t =
        function_type_t<decltype(detail::FunctionPointerToType(0, F))>;

    namespace detail
    {
        template <typename Func>
        struct BasicFunctionTraits;

        template <typename Ret, typename... Args>
        struct BasicFunctionTraits<Ret(Args...)>
        {
            using args        = TypeList<Args...>;
            using return_type = Ret;
        };
    }    // namespace detail

    /**
     * @brief extract function info from function type
     *
     * @tparam Func
     */
    template <typename Func>
    struct FunctionTraits;

    template <typename Ret, typename... Args>
    struct FunctionTraits<Ret(Args...)> : detail::BasicFunctionTraits<Ret(Args...)>
    {
        using type    = Ret(Args...);
        using pointer = Ret (*)(Args...);

        static constexpr bool is_member = false;
        static constexpr bool is_const  = false;
    };

    template <typename Ret, typename Class, typename... Args>
    struct FunctionTraits<Ret (Class::*)(Args...)> : detail::BasicFunctionTraits<Ret(Args...)>
    {
        using type    = Ret (Class::*)(Args...);
        using pointer = type;

        static constexpr bool is_member = true;
        static constexpr bool is_const  = false;
    };

    template <typename Ret, typename Class, typename... Args>
    struct FunctionTraits<Ret (Class::*)(Args...) const>
      : detail::BasicFunctionTraits<Ret(Args...)>
    {
        using type    = Ret (Class::*)(Args...) const;
        using pointer = type;

        static constexpr bool is_member = true;
        static constexpr bool is_const  = true;
    };

    namespace detail
    {
        template <auto F>
        struct FunctionPointerTraits : FunctionTraits<function_type_from_pointer_t<F>>
        { };
    }    // namespace detail

    /**
     * @brief extract function info from function pointer
     */
    template <auto F>
    using FunctionPointerTraits = detail::FunctionPointerTraits<F>;

    /**
     * @brief check a type is a function or function pointer
     * @tparam T
     */
    template <typename T>
    constexpr bool is_function_v =
        std::is_function_v<T> || std::is_member_function_pointer_v<T>;

}    // namespace mirage::util
