#include <benchmark/benchmark.h>

#include "hash_table.hpp"
#include "hash_table_impl.hpp"

static void htableBench(benchmark::State& addit) {
  auto len = static_cast<double>(addit.range(0));

  for (auto _ : addit) {
    HashTable<int> first;
    double startval = 1;
    change_realloc_size(first, static_cast<double>(len / 10), startval);

    for (size_t i = 0; i < 1e6; ++i) {
      first.addelem(i);
    }
  }
}

BENCHMARK(htableBench)->DenseRange(1, 30)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();