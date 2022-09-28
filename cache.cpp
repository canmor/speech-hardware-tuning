#include <benchmark/benchmark.h>

constexpr int cache_line_size = 512;
constexpr int cols = cache_line_size / sizeof(int);
constexpr int cache_size = 4096 * 1024;
constexpr int rows = cache_size / cache_line_size;
int data[rows][cols];
static bool initialized = false;

static void GenerateData(const benchmark::State &state) {
    if (initialized)
        return;

    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c)
            data[r][c] = rand();
    initialized = true;
}

static void RowByRow(benchmark::State &state) {
    int max = 0;
    for (auto _: state) {
        for (int r = 0; r < rows; ++r)
            for (int c = 0; c < cols; ++c)
                max = std::max(max, data[r][c]);
        benchmark::DoNotOptimize(max);
    }
}

BENCHMARK(RowByRow)->Setup(GenerateData);

static void ColByCol(benchmark::State &state) {
    int max = 0;
    for (auto _: state) {
        for (int c = 0; c < cols; ++c)
            for (int r = 0; r < rows; ++r)
                max = std::max(max, data[r][c]);
        benchmark::DoNotOptimize(max);
    }
}

BENCHMARK(ColByCol)->Setup(GenerateData);