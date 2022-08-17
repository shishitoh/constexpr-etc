#include <iostream>
#include <tuple>

#include "../prime.hpp"

int main() {
    using namespace prime;

    constexpr std::size_t pmax = 100;
    constexpr auto tuple = tuple_sieve<pmax>();
    static_assert(
        tuple_sieve<100>() ==
            (decltype(tuple)){
                2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
                31, 37, 41, 43, 47, 53, 59, 61, 67,
                71, 73, 79, 83, 89, 97});

}

