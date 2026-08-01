#ifndef RANDOM_H
#define RANDOM_H

#include <chrono>
#include <random>

namespace Random {
    inline std::mt19937_64 mt {
        static_cast<std::seed_seq::result_type> (
            std::chrono::steady_clock::now().time_since_epoch().count()
        )
    };

    template<typename T>
    T get(T min, T max) {
        return std::uniform_int_distribution<T>{min, max}(mt);
    }
}

#endif
