#ifndef BENCH_CACHE_H
#define BENCH_CACHE_H
#include <fstream>
#include <iostream>

// macos: >$ sysctl -a | grep hw.cache
static int cache_line_size = 128;
static int cache_size = 64 * 1024;
constexpr int cache_way = 8;

static inline void fetch_cache_config() {
    {
        std::ifstream is("/sys/devices/system/cpu/cpu0/cache/index0/coherency_line_size");
        is >> cache_line_size;
        if (is) {
            std::cout << cache_line_size << std::endl;
        }
    }

    {
        std::ifstream is("/sys/devices/system/cpu/cpu0/cache/index1/size");
        is >> cache_size;
        if (is) {
            std::cout << cache_size << "KiB" << std::endl;
            cache_size *= 1024;
        }
    }
}
#endif //BENCH_CACHE_H
