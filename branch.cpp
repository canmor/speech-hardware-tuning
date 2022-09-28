#include <benchmark/benchmark.h>
#include <numeric>
#include <random>


struct Salary {
    static constexpr int tax_bracket = 10'000;
    int total;

    Salary(int total) : total(total) {}

    int GetTax() const {
        int tax;
        double tax_rate_tier1 = 0.05;
        double tax_rate_tier2 = 0.10;
        if (total > tax_bracket) {
            // x + promotion_threshold * rate1 + (x - promotion_threshold) * rate2 = y;
            // x + x * rate2 + promotion_threshold * rate1 - promotion_threshold * rate2 = y;
            // x = (y + promotion_threshold * rate2 - promotion_threshold * rate1) / (1+rate2);
            //  tax = y - x = y - (y + tax_bracket * rate2 - tax_bracket * rate1) / (1+rate2);
            //              = y - (y + tax_bracket * rate2 - tax_bracket * rate1) / (1+rate2);
            tax = total - (total + tax_bracket * tax_rate_tier2 - tax_bracket * tax_rate_tier1) / (1 + tax_rate_tier2);
        } else {
            //x + x * rate = y
            //x * (1+rate) = y
            // x = y / (1+rate)
            // tax = y - x = y - y / (1+rate)
            //  = (y*(1+rate) - y) / (1+rate)
            //  = y*rate / (1+rate)
            tax = total * tax_rate_tier1 / (1 + tax_rate_tier1);
        }
        return tax;
    }
};

std::vector<Salary> salaries;
std::vector<Salary> ordered_salaries;

static void GenerateData(const benchmark::State &state) {
    if (!salaries.empty())
        return;

    constexpr int count = 100'000;
    salaries.reserve(count);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, Salary::tax_bracket * 2);
    for (int i = 0; i < count; ++i)
        salaries.emplace_back(dis(gen));

    ordered_salaries = salaries;
    std::sort(ordered_salaries.begin(), ordered_salaries.end(),
              [](auto &a, auto &b) { return a.total < b.total; });
}

static void BranchUnordered(benchmark::State &state) {
    for (auto _: state) {
        auto sum = std::accumulate(salaries.begin(), salaries.end(), 0,
                                   [](auto sum, auto &o) { return sum + o.GetTax(); });
        benchmark::DoNotOptimize(sum);
    }
}

BENCHMARK(BranchUnordered)->Setup(GenerateData);

static void BranchOrdered(benchmark::State &state) {
    for (auto _: state) {
        auto sum = std::accumulate(ordered_salaries.begin(), ordered_salaries.end(), 0,
                                   [](auto sum, auto &o) { return sum + o.GetTax(); });
        benchmark::DoNotOptimize(sum);
    }
}

BENCHMARK(BranchOrdered)->Setup(GenerateData);