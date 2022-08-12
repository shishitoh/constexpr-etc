#include <type_traits>
#include <tuple>

#include "../tuple_traits.hpp"

int main() {
    using namespace tuple_traits;

    static_assert(std::is_same_v<
                      std::tuple<int, int, int>,
                      homotuple<int, 3>
                  >, "homotuple");
     static_assert(std::is_same_v<
                      std::tuple<>,
                      homotuple<int, 0>
                  >, "homotuple");
    static_assert(std::is_same_v<
                      std::tuple<>,
                      tuples_cat<>
                  >, "tuples_cat");
    static_assert(std::is_same_v<
                      std::tuple<char, const int, float*, int&>,
                      tuples_cat<std::tuple<char>, std::tuple<const int>, std::tuple<float*, int&>>
                  >, "tuples_cat");
    static_assert(meta_apply_v<
                      std::is_same,
                      std::tuple<int, int>
                  >, "meta_apply");
    static_assert(std::is_same_v<
                      std::tuple<long, int*, const char*, float[3]>,
                      swapped_tuple<
                          std::tuple<long, float[3], const char*, int*>, 3, 1
                      >
                  >, "swapped_tuples");
    constexpr int A[5] = {2, 3, 5, 7, 11};
    static_assert(darray_to_tuple<int, 5>(A) == std::make_tuple(2, 3, 5, 7, 11));
}
