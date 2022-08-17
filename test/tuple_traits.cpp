#include <type_traits>
#include <tuple>

#include "../tuple_traits.hpp"

consteval bool check() {
    using namespace tuple_traits;
    int *A = new int[5]{3, 1, 4, 1, 5};
    bool t = darray_to_tuple<int, 5>(A) == std::make_tuple(3, 1, 4, 1, 5);
    delete[] A;
    return t;
}

int main() {
    using namespace tuple_traits;

    static_assert(std::is_same_v<
                      std::tuple<int, int, int>,
                      homotuple<int, 3>
                  >);
     static_assert(std::is_same_v<
                      std::tuple<>,
                      homotuple<int, 0>
                  >);
    static_assert(std::is_same_v<
                      std::tuple<>,
                      tuples_cat<>
                  >);
    static_assert(std::is_same_v<
                      std::tuple<char, const int, float*, int&>,
                      tuples_cat<std::tuple<char>, std::tuple<const int>, std::tuple<float*, int&>>
                  >);
    static_assert(meta_apply_v<
                      std::is_same,
                      std::tuple<int, int>
                  >);
    static_assert(std::is_same_v<
                      std::tuple<long, int*, const char*, float[3]>,
                      swapped_tuple<
                          std::tuple<long, float[3], const char*, int*>, 3, 1
                      >
                  >);
    constexpr int A[5] = {2, 3, 5, 7, 11};
    static_assert(darray_to_tuple<int, 5>(A) == std::make_tuple(2, 3, 5, 7, 11));
    static_assert([]() {
                      int *A = new int[5]{3, 1, 4, 1, 5};
                      A[1] = 10;
                      homotuple<int, 5> t = darray_to_tuple<int, 5>(A);
                      delete[] A;
                      return t;
                  }() == std::make_tuple(3, 10, 4, 1, 5));
}
