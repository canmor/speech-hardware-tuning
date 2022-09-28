#include <benchmark/benchmark.h>

namespace fizzbuzz {
    std::string SayInfrequentFirst(int number) {
        if (number <= 0) {
            return "invalid number";
        }
        if (number % 15 == 0) {
            return "FizzBuzz";
        }
        if (number % 3 == 0) {
            return "Fizz";
        }
        if (number % 5 == 0) {
            return "Buzz";
        }
        return std::to_string(number);
    }

    std::string SayFrequentFirst(int number) {
        if (number > 0) {
            if (number % 15 == 0) {
                return "FizzBuzz";
            }
            if (number % 3 == 0) {
                return "Fizz";
            }
            if (number % 5 == 0) {
                return "Buzz";
            }
            return std::to_string(number);
        }
        return "invalid number";
    }
};

constexpr int repetition = 30'000;

static void BranchInfrequentFirst(benchmark::State &state) {
    for (auto _: state) {
        for (int i = 0; i < repetition; ++i) {
            benchmark::DoNotOptimize(fizzbuzz::SayInfrequentFirst(i));
        }
    }
}

BENCHMARK(BranchInfrequentFirst);

static void BranchFrequentFirst(benchmark::State &state) {
    for (auto _: state) {
        for (int i = 0; i < repetition; ++i) {
            benchmark::DoNotOptimize(fizzbuzz::SayFrequentFirst(i));
        }
    }
}

BENCHMARK(BranchFrequentFirst);