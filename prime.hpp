#include <tuple>

#include "tuple_traits.hpp"

namespace prime {
    using namespace tuple_traits;

    /* tuple_sieve */
    template<std::size_t n>
    struct sieve_flag {

        static constexpr std::size_t size = n;
        bool flags[size/2];

        constexpr sieve_flag() {

            flags[0] = 0;
            for (std::size_t i = 1; i < size/2; ++i) {
                flags[i] = 1;
            }

            for (std::size_t i = 1, j; (j=2*i*i+2*i) < size/2; ++i) {
                if (flags[i]) {
                    for (; j < size/2; j+=2*i+1) {
                        flags[j] = 0;
                    }
                }
            }
        }

        constexpr std::size_t get_size() const {
            std::size_t ret = 1;
            for (std::size_t i = 1; i < size/2; ++i) {
                ret+=flags[i];
            }
            return ret;
        }
    };

    template<std::size_t n>
    constexpr auto tuple_sieve() {

        constexpr sieve_flag<n> flags{};
        constexpr std::size_t Size = flags.get_size();

        int *P = new int[Size], *p = P;
        (*p++) = 2;
        for (std::size_t i = 0; i < n/2; ++i) {
            if (flags.flags[i]) {
                (*p++) = 2*i+1;
            }
        }
        auto tuple = darray_to_tuple<int, Size>(P);
        delete[] P;

        return tuple;
    }

    constexpr auto prime_factorization(std::size_t n) {

    }

}

