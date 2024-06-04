#pragma once


#include <tuple>
#include <variant>

namespace mirage::util
{
    /**
     * @brief a type container to store types
     * @tparam Ts types
     */
    template <typename... Ts>
    struct TypeList
    {
        using self_type                   = TypeList<Ts...>;
        static constexpr std::size_t size = sizeof...(Ts);
    };

    namespace detail
    {
        template <typename, std::size_t>
        struct ListElement;

        template <template <typename...> typename ListType,
                  typename T,
                  typename... Ts,
                  std::size_t N>
        struct ListElement<ListType<T, Ts...>, N>
          : ListElement<ListType<Ts...>, N - 1>
        { };

        template <template <typename...> typename ListType,
                  typename T,
                  typename... Ts>
        struct ListElement<ListType<T, Ts...>, 0>
        {
            using type = T;
        };

        template <typename>
        struct ListSize;

        template <template <typename...> typename ListType, typename... Ts>
        struct ListSize<ListType<Ts...>>
        {
            static constexpr std::size_t value = sizeof...(Ts);
        };

        template <typename>
        struct ListHead;

        template <template <typename...> typename ListType,
                  typename T,
                  typename... Ts>
        struct ListHead<ListType<T, Ts...>>
        {
            using type = T;
        };

        template <typename>
        struct ListTail;

        template <template <typename...> typename ListType,
                  typename T,
                  typename... Ts>
        struct ListTail<ListType<T, Ts...>>
        {
            using type = ListType<Ts...>;
        };

        template <typename List, typename T>
        struct ListAddToFirst;

        template <template <typename...> typename ListType,
                  typename... Ts,
                  typename T>
        struct ListAddToFirst<ListType<Ts...>, T>
        {
            using type = ListType<T, Ts...>;
        };
    }    // namespace detail

    /**
     * @brief get the N-th element in type list (from type_list or std::tuple or
     * others
     *
     * @tparam N the element index
     * @tparam List
     */
    template <typename List, std::size_t N>
    using list_element_t = typename detail::ListElement<List, N>::type;

    /**
     * @brief get the length of type list
     * @tparam List
     */
    template <typename List>
    constexpr std::size_t list_size_v = detail::ListSize<List>::value;

    /**
     * @brief check whether a list is empty
     * @tparam List
     */
    template <typename List>
    constexpr bool is_list_empty = list_size_v<List> == 0;

    /**
     * @brief get head element from type list
     */
    template <typename List>
    using list_head_t = typename detail::ListHead<List>::type;

    /**
     * @brief  get tail element from type list
     */
    template <typename List>
    using list_tail_t = typename detail::ListTail<List>::type;

    template <typename List, typename T>
    using list_add_to_first_t = typename detail::ListAddToFirst<List, T>::type;

    namespace detail
    {
        template <typename List, std::size_t N, template <typename> typename F>
        struct apply_to_element
        {
            using type = F<list_element_t<List, N>>;
        };
    }    // namespace detail

    /**
     * @brief apply a template function struct to a list element
     *
     * @tparam List
     * @tparam N
     * @tparam F  like `template<typename> struct { constexpr auto value = XXX
     * };`, recieve a type and give a value
     */
    template <typename List, std::size_t N, template <typename> typename F>
    using apply_to_element_t = typename detail::apply_to_element<List, N, F>::type;

    namespace detail
    {
        template <typename List, template <typename> typename F>
        struct Disjunction
        {
            static constexpr bool value =
                F<list_head_t<List>>::value
                || Disjunction<list_tail_t<List>, F>::value;
        };

        template <template <typename...> typename ListType,
                  template <typename>
                  typename F>
        struct Disjunction<ListType<>, F>
        {
            static constexpr bool value = false;
        };

        template <typename List, template <typename> typename F>
        struct Conjunction
        {
            static constexpr bool value =
                F<list_head_t<List>>::value
                && Disjunction<list_tail_t<List>, F>::value;
        };

        template <template <typename...> typename ListType,
                  template <typename>
                  typename F>
        struct Conjunction<ListType<>, F>
        {
            static constexpr bool value = true;
        };

        template <typename List1, typename List2>
        struct Concat;

        template <template <typename...> typename ListType,
                  typename... Ts1,
                  typename... Ts2>
        struct Concat<ListType<Ts1...>, ListType<Ts2...>>
        {
            using type = ListType<Ts1..., Ts2...>;
        };

        template <typename TypeList>
        struct TypeListToTuple;

        template <typename... Ts>
        struct TypeListToTuple<TypeList<Ts...>>
        {
            using type = std::tuple<Ts...>;
        };

        template <typename Tuple>
        struct TupleToTypeList;

        template <typename... Ts>
        struct TupleToTypeList<std::tuple<Ts...>>
        {
            using type = TypeList<Ts...>;
        };

        template <typename List, template <typename> typename F>
        struct ListFilter;

        template <template <typename...> typename ListType,
                  typename T,
                  typename... Ts,
                  template <typename>
                  typename F>
        struct ListFilter<ListType<T, Ts...>, F>
        {
            using type = std::conditional_t<
                F<T>::value,
                list_add_to_first_t<typename ListFilter<ListType<Ts...>, F>::type,
                                    T>,
                typename ListFilter<ListType<Ts...>, F>::type>;
        };

        template <template <typename...> typename ListType,
                  template <typename>
                  typename F>
        struct ListFilter<ListType<>, F>
        {
            using type = ListType<>;
        };
    }    // namespace detail

    /**
     * @brief work as std::disjunction: use function struct F to check any of
     * element is true
     *
     * @tparam List
     * @tparam F  receive a type and give a constexpr static bool value;
     */
    template <typename List, template <typename> typename F>
    constexpr bool disjunction_v = detail::Disjunction<List, F>::value;

}    // namespace mirage::util
