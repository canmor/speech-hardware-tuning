#include <benchmark/benchmark.h>

struct BigObject {
    int valid;
    int other_fields[32]; // make this object bigger than cache line

    BigObject(int valid) : valid(valid) {}

    bool IsValid() const { return valid; };
};

std::vector<BigObject> data_set;

static void GenerateData(const benchmark::State &state) {
    if (!data_set.empty())
        return;

    constexpr int count = 100'000;
    data_set.reserve(count);
    for (int i = 0; i < count; ++i)
        data_set.emplace_back(rand() & 1);
}

static void CountWithBigObject(benchmark::State &state) {
    for (auto _: state) {
        auto is_valid = [](auto &o) { return o.IsValid(); };
        auto count = std::count_if(data_set.begin(), data_set.end(), is_valid);
        benchmark::DoNotOptimize(count);
    }
}

BENCHMARK(CountWithBigObject)->Setup(GenerateData);

static void CountWithFlags(benchmark::State &state) {
    std::vector<int> valid_set;
    valid_set.reserve(data_set.size());
    auto valid_flag = [](auto &d) { return d.valid; };
    std::transform(data_set.begin(), data_set.end(), std::back_inserter(valid_set), valid_flag);
    for (auto _: state) {
        auto is_valid = [](auto o) { return o; };
        auto count = std::count_if(valid_set.begin(), valid_set.end(), is_valid);
        benchmark::DoNotOptimize(count);
    }
}

BENCHMARK(CountWithFlags)->Setup(GenerateData);