#include <benchmark/benchmark.h>
#include "cache.h"

constexpr int factor = 64;

static inline unsigned calculate_column_size() {
    return cache_size / cache_way / sizeof(int);
}

static inline unsigned calculate_row_size() {
    return cache_way * factor;
}

static auto col_size = calculate_column_size();
static auto row_size = calculate_row_size();

std::vector<int> data;

static void GenerateData(const benchmark::State &state) {
    if (!data.empty())
        return;

    fetch_cache_config();
    col_size = calculate_column_size();
    row_size = calculate_row_size();

    data.assign(row_size * col_size, 0);
    for (int row = 0; row < row_size; ++row)
        for (int col = 0; col < col_size; ++col)
            data[row * col_size + col] = rand();
}

static void RowByRow(benchmark::State &state) {
    int max = 0;
    for (auto _: state) {
        for (int row = 0; row < row_size; ++row)
            for (int col = 0; col < col_size; ++col)
                max = std::max(max, data[row * col_size + col]);
        benchmark::DoNotOptimize(max);
    }
}

BENCHMARK(RowByRow)->Setup(GenerateData);

static void ColByCol(benchmark::State &state) {
    int max = 0;
    for (auto _: state) {
        for (int col = 0; col < col_size; ++col)
            for (int row = 0; row < row_size; ++row)
                max = std::max(max, data[row * col_size + col]);
        benchmark::DoNotOptimize(max);
    }
}

BENCHMARK(ColByCol)->Setup(GenerateData);