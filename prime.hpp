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

    constexpr std::size_t modsum(std::size_t a, std::size_t b, std::size_t n) {
        return (a == 0 || b < -a) ? (a+b) % n : (a+b-n) % n;
    }

    constexpr std::size_t modprod(std::size_t a, std::size_t b, std::size_t n) {
        std::size_t ret = 0;
        while (b) {
            if (b&1) {
                ret = modsum(ret, a, n);
            }
            a = modsum(a, a, n);
            b >>= 1;
        }
        return ret;
    }

    constexpr std::size_t modpow(std::size_t a, std::size_t b, std::size_t n) {
        std::size_t ret = 1;
        while (b) {
            if (b&1) {
                ret = modprod(ret, a, n);
            }
            a = modprod(a, a, n);
            b >>= 1;
        }
        return ret;
    }

    /* n = 2^s*dを満たす */
    constexpr bool miller_rabin_core(std::size_t n, std::size_t a, std::size_t s, std::size_t d) {

        std::size_t y = modpow(a, d, n);
        if (y == 1) {
            return true;
        }
        for (std::size_t r = 0; r < s; ++r) {
            if (y == n-1) {
                return true;
            }
            y = modprod(y, y, n);
        }
        return false;
    }

    constexpr is_prime(std::size_t n) {

        if (n == 2) {
            return true;
        } else if (n==0 || n==1 || !(n&1)) {
            return false;
        }

        std::size_t A[7];
        std::size_t As;

        if (n < 1373653ULL) {
            A[0] = 2; A[1] = 3; As = 2;
        } else if (n < 25326001ULL) {
            A[0] = 2; A[1] = 3; A[2] = 5; As = 3;
        } else if (n < 118670087467) {
            A[0] = 2; A[1] = 3; A[2] = 5; A[3] = 7;
            As = 4;
        } else if (n < 2152302898747ULL) {
            A[0] = 2; A[1] = 3; A[2] = 5; A[3] = 7;
            A[4] = 11; As = 5;
        } else if (n < 3474749660383ULL) {
            A[0] = 2; A[1] = 3; A[2] = 5; A[3] = 7;
            A[4] = 11; A[5] = 13; As = 6;
        } else if (n < 341550071728321ULL) {
            A[0] = 2; A[1] = 3; A[2] = 5; A[3] = 7;
            A[4] = 11; A[5] = 13; A[6] = 17; As = 7;
       } else {
           /* not implemented */
            return false;
        }

        std::size_t d = n-1;
        std::size_t s = 0;
        while (!(d&1)) {
            ++s;
            d >>= 1;
        }
        for (std::size_t Ass = 0; Ass < As; ++Ass) {
            if (A[Ass] >= n) {
                break;
            }
            if (!miller_rabin_core(n, A[Ass], s, d)) {
                return false;
            }
        }
        return true;
    }
}

