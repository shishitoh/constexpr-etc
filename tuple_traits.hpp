#include <iostream>
#include <tuple>

namespace tuple_traits {

    /* struct homotuple
    第一引数の型を第二引数の数字だけ並べたtuple型を返す
    繰り返し二乗法で実装
    ex. homotuple<int, 5>
        -> std::tuple<int, int, int, int, int> */
    template<typename... Args>
    struct typeholder {};

    template<std::size_t N, bool B, typename S, typename... Args>
    struct homotuple_impl {};

    template<std::size_t N, typename... pArgs, typename... Args>
    struct homotuple_impl<N, false, typeholder<pArgs...>, Args...> {
        using type = typename homotuple_impl<
            (N>>1), (N&2), typeholder<pArgs..., pArgs...>, Args...>::type;
    };

    template<std::size_t N, typename... pArgs, typename... Args>
    struct homotuple_impl<N, true, typeholder<pArgs...>, Args...> {
        using type = typename homotuple_impl<
            (N>>1), (N&2), typeholder<pArgs..., pArgs...>, Args..., pArgs...>::type;
    };

    template<typename... pArgs, typename... Args>
    struct homotuple_impl<1, true, typeholder<pArgs...>, Args...> {
        using type = std::tuple<Args..., pArgs...>;
    };

    template<typename... pArgs, typename... Args>
    struct homotuple_impl<0, false, typeholder<pArgs...>, Args...> {
        using type = std::tuple<Args...>;
    };

    template<typename T, std::size_t N>
    using homotuple = typename homotuple_impl<N, N&1, typeholder<T>>::type;

    /* struct tuples_cat
    引数に渡されたtupleを全て結合する
    ex. tuples_cat<std::tuple<int>, std::tuple<float, long*>, std::tuple<>>
        -> std::tuple<int, float, long*> */
    template<typename... Tuples>
    struct tuples_cat_impl;

    template<>
    struct tuples_cat_impl<> {
        using type = std::tuple<>;
    };

    template<typename... Args>
    struct tuples_cat_impl<std::tuple<Args...>> {
        using type = std::tuple<Args...>;
    };

    template<typename... Args1, typename... Args2, typename... Tuples>
    struct tuples_cat_impl<std::tuple<Args1...>, std::tuple<Args2...>, Tuples...> {
        using type = tuples_cat_impl<std::tuple<Args1..., Args2...>, Tuples...>::type;
    };

    template<typename... Tuples>
    using tuples_cat = tuples_cat_impl<Tuples...>::type;

    /* struct meta_apply
    templateの第一引数のメタ関数にArgs...を適用させたものを返す
    Args...はtuple<Args...>とパックして渡す
    ex. meta_apply<std::is_same, std::tuple<int, float>>
        -> std::is_same<int, float> */
    template<template<typename... Args> typename T1, typename T2>
    struct meta_apply_impl;

    template<template<typename... Args1> typename T, typename... Args2>
    struct meta_apply_impl<T, std::tuple<Args2...>> {
        using type = T<Args2...>;
    };

    template<template<typename... Args> typename T1, typename T2>
    using meta_apply = typename meta_apply_impl<T1, T2>::type;

    template<template<typename... Args> typename T1, typename T2>
    using meta_apply_t = typename meta_apply<T1, T2>::type;

    template<template<typename... Args> typename T1, typename T2>
    constexpr auto meta_apply_v = meta_apply<T1, T2>::value;

    /* struct swapped_tuple
    第一引数のtupleに対して第2, 第3引数数字の位置の型を交換した新しいtupleを返す
    ex. swapped_tuple<std::tuple<int, double&, char*>, 2, 1>
        -> std::tuple<int, char*, double&> */
    template<std::size_t TS, typename T, std::size_t N, std::size_t M, std::size_t S, typename... Args>
    struct swapped_tuple_swapping {
        using type = typename swapped_tuple_swapping<TS, T, N, M, S+1, Args..., std::tuple_element_t<S, T>>::type;
    };

    template<std::size_t TS, typename T, std::size_t N, std::size_t M, typename... Args>
    struct swapped_tuple_swapping<TS, T, N, M, N, Args...> {
        using type = typename swapped_tuple_swapping<TS, T, N, M, N+1, Args..., std::tuple_element_t<M, T>>::type;
    };

    template<std::size_t TS, typename T, std::size_t N, std::size_t M, typename... Args>
    struct swapped_tuple_swapping<TS, T, N, M, M, Args...> {
        using type = typename swapped_tuple_swapping<TS, T, N, M, M+1, Args..., std::tuple_element_t<N, T>>::type;
    };

    template<std::size_t TS, typename T, std::size_t N, std::size_t M, typename... Args>
    struct swapped_tuple_swapping<TS, T, N, M, TS, Args...> {
        using type = std::tuple<Args...>;
    };

    template<typename T, std::size_t N, std::size_t M>
    struct swapped_tuple_impl;

    template<typename... Args, std::size_t N, std::size_t M>
    struct swapped_tuple_impl<std::tuple<Args...>, N, M> {
        using type = swapped_tuple_swapping<sizeof...(Args), std::tuple<Args...>, N, M, 0>::type;
    };

    template<typename... Args, std::size_t N>
    struct swapped_tuple_impl<std::tuple<Args...>, N, N> {
        using type = std::tuple<Args...>;
    };

    template<typename T, std::size_t N, std::size_t M>
    using swapped_tuple = typename swapped_tuple_impl<T, N, M>::type;

    /* darray_to_tuple
    dynamic_arrayをtupleに変換する */
    template<typename T, std::size_t Low, std::size_t High, std::size_t Size>
    constexpr void darray_to_tuple_impl(homotuple<T, Size> &tuple, const T *ptr) {

        static_assert(Low <= High);

        if constexpr (Low == High) {
            ;
        } else if (High - Low == 1) {
            std::get<Low>(tuple) = ptr[Low];
        } else {
            darray_to_tuple_impl<T, Low, (Low+High)/2, Size>(tuple, ptr);
            darray_to_tuple_impl<T, (Low+High)/2, High, Size>(tuple, ptr);
        }
    }

    template<typename T, std::size_t Size>
    constexpr homotuple<T, Size> darray_to_tuple(const T *ptr) {
        homotuple<T, Size> tuple;
        darray_to_tuple_impl<T, 0, Size, Size>(tuple, ptr);
        return tuple;
    }

    /* tupleの中身をostreamで全て出力する */
    template<std::size_t Low, std::size_t High, typename... Args>
    void ostream_impl(std::ostream &stream, const std::tuple<Args...> &tuple) {
        if constexpr (Low+1 > High) {
            ;
        } else if (Low+1 == High) {
            if constexpr (Low == 0) {
                stream << std::get<Low>(tuple);
            } else {
                stream << ", " << std::get<Low>(tuple);
            }
        } else {
            ostream_impl<Low, (Low+High)/2, Args...>(stream, tuple);
            ostream_impl<(Low+High)/2, High, Args...>(stream, tuple);
        }
    }

    template<typename... Args>
    std::ostream &operator<<(std::ostream &stream, const std::tuple<Args...> &tuple) {
        stream << "tuple(";
        ostream_impl<0, sizeof...(Args)>(stream, tuple);
        stream << ")";
        return stream;
    }
}

